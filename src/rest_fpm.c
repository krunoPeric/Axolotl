/**
 * Harlan James <root@aboyandhisgnu.org> <osurobotics.club>
 *
 * This file is part of Axolotl.
 *
 * Axolotl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Axolotl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Axolotl.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "rest_fpm.h"

#include "global.h"
#include "secio.h"
#include "serialize.h"

#include <fcgi_stdio.h>
#include <curl/curl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define USE_DEMO_WRAPPER

/**
 * DATABASE_URL - Base URL for database wrapper
 */
#ifdef USE_DEMO_WRAPPER
	const char * const DATABASE_URL = "https://script.google.com/macros/s/AKfycbwh53E9u-ADQyE996zc0x6PlmLvnEmssMSO7pAGpqCPhPbI0Fk/exec";
#endif

/**
 * explode - splits a URI into @tokens -- delimited by '/' and '0'.
 *
 * @query: the URI passed to FCGI. Must NOT be modified
 */
__attribute__((const, nonnull))
struct query_parser explode(const char * const query)
{
	struct query_parser result = {0};
	size_t seek_state;
	size_t i, t;

	if (query == NULL)
		return result;

	// Calculate number of tokens
	i = 0;
	while (query[i] != '\0')
	{
		if (query[i] == '/' && query[i+1] != '\0')
		{
			++result.count;
		}
		
		++i;
	}

	if (result.count == 0)
		return result;

	result.tokens = malloc(result.count*sizeof(struct token));

	
	// Parse query for token positions
	i = 0;
	t = 0;
	seek_state = 0;
	while (query[i] != '\0')
	{
		if (seek_state == 0)
		{	// Seek start
			if (query[i] == '/' &&  query[i+1] != '\0')
			{
				result.tokens[t].start = i+1;
				seek_state = 1;
			}
		}
		else
		{	// Seek end
			if (query[i+1] == '/' || query[i+1] == '\0')
			{
				result.tokens[t].end = i;
				result.tokens[t].size = i - result.tokens[t].start + 1;
				seek_state = 0;
				++t;
			}
		}

		++i;
	}

	return result;
}

/**
 * build_api_query - generates the URI used to query the database wrapper
 *
 * @parser: pre-filled @query_parser for the source @query
 * @query: source
 */
__attribute__((const, nonnull))
char * build_api_query(struct query_parser parser, const char * const uri)
{
	char *result;
	char r;
	size_t len;
	size_t i, t;
	size_t offs;

	if (uri == NULL)
		return NULL;

	len = strlen(uri);
	if (len <= 1 || parser.count < 1)
		return NULL;

	offs = strlen(DATABASE_URL);
	len += offs;

	result = malloc((len+1)*sizeof(char));
	strcpy(result, DATABASE_URL);
	strcat(result, uri);

	result[offs] = '?';

	for (i = 1; i < parser.count; i++)
	{
		if ((i & 0b0001) == 1)
			r = '=';
		else
			r = '&';

		t = (offs + parser.tokens[i].start) - 1;
		result[t] = r;
	}

	return result;
}

int main(int argc, char *argv[])
{
	int exit_status = EXIT_SUCCESS;

	char *result;
	char *request_uri;
	char *api_query;
	struct query_parser request;

	while (FCGI_Accept() >= 0)
	{
		printf("Status: 200\r\n");
		printf("Content-type: application/json\r\n");
		printf("\r\n");

		request_uri = getenv("REQUEST_URI");
		if (request_uri == NULL)
		{
			result = serialize("error", "could not get environment");
			printf(result);
			continue;
		}
		
		request = explode(request_uri);
		if (request.count == 0)
		{
			result = serialize("error", "invalid request uri");
			printf(result);
			continue;
		}
	
		api_query = build_api_query(request, request_uri);
		result = serialize("api_call", api_query);

		print_s(result);

		destroy(request.tokens);
		destroy(api_query);
		destroy(result);
	}

	return exit_status;
}
