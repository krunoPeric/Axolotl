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
#include "rcurl.h"
#include "secio.h"
#include "serialize.h"
#ifndef HEADLESS
	#include "termio.h"
#endif

#include <fcgi_stdio.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
__attribute__((nonnull))
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
 * @query: source uri
 */
__attribute__((nonnull))
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

__attribute__((noreturn))
void die()
{
	exit(EXIT_FAILURE);
}

void int_handler(int signo)
{
	if (signo != SIGINT)
	{
		return;
	}

	tprintfb("Caught SIGINT", "Exiting gracefully");

	rcurl_destroy();
	die();
}

static time_t raw_time;
void init_or_die()
{
	raw_time = time(NULL);

	// Initialize cURL
	if (rcurl_init() == 0)
	{
		die();
	}

	// Gracefully exit on interrupt
	if (signal(SIGINT, int_handler) == SIG_ERR)
	{
		die();
	}
}

/**
 * on_accept - logs and prints information when a client connects
 */
int on_accept() {
	int success = 1;

	struct tm tm = *localtime(&raw_time);

	// Print request information to terminal
	#ifndef HEADLESS
		_tprintfb(getenv("REQUEST_METHOD"), getenv("REMOTE_ADDR"));
		tprintf(" %d-%d-%d+%d:%d:%d\n",
			tm.tm_year + 1900,
			tm.tm_mon + 1,
			tm.tm_mday,
			tm.tm_hour,
			tm.tm_min,
			tm.tm_sec);
		tprintfb("Request", getenv("REQUEST_URI"));
	#endif

	return success;
}

int main(int argc, char *argv[])
{
	int exit_status = EXIT_SUCCESS;

	char *result;
	char *request_uri;
	char *api_query;
	struct query_parser request;

	tprintf("Starting Axolotl %s compiled on %s %s\n", VERSION,  __DATE__, __TIME__);

	init_or_die();

	while (FCGI_Accept() >= 0)
	{
		// Log client connection
		(void) on_accept();

		// TODO Defer writing status until status is known, e.g., 400, etc.
		printf("Status: 200\r\n");
		printf("Content-type: application/json\r\n");
		printf("\r\n");

		request_uri = getenv("REQUEST_URI");
		if (request_uri == NULL)
		{
			result = serialize("error", "could not get environment");
			printf(result);
			destroy(result);

			continue;
		}
		
		request = explode(request_uri);
		if (request.count == 0)
		{
			result = serialize("error", "invalid request uri");
			printf(result);
			destroy(result);

			continue;
		}

		// Generate a request to the demo database wrapper. In the future this
		// will be moved to a locally cached NoSQL database.
		api_query = build_api_query(request, request_uri);
		if (api_query == NULL)
		{
			result = serialize("error", "could not build request");
			print_s(result);
			destroy(result);

			continue;
		}

		// Query the database
		result = rcurl_fetch(api_query);
		if (result == NULL)
		{
			result = serialize("error", "could not connect to database :(");
			print_s(result);
			destroy(result);

			continue;
		}

		// Safely print the result
		// TODO: Add this to every method which writes to a device or remove it
		//       entirely. In the same vein -- malloc() should probably be moved
		//       to use calloc() instead.
		print_s(result);

		// End of client session
		destroy(request.tokens);
		destroy(api_query);
		destroy(result);
	}

	// Free cURL memory
	rcurl_destroy();

	return exit_status;
}
