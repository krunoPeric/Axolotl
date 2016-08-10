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
#include "rcurl.h"

#include "global.h"
#include "secio.h"

#include <stdlib.h>
#include <curl/curl.h>

#define CURL_FAILURE 0
#define CURL_SUCCESS 1

static CURL *handle;
static struct curl_slist *headers;

int rcurl_init()
{
	if (handle == NULL)
	{
		handle = curl_easy_init();
	}
	else
	{
		return CURL_FAILURE;
	}

	// Set Content Type
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");

	// Set cURL options
	curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);

	return CURL_SUCCESS;
}

void rcurl_destroy()
{
	curl_easy_cleanup(handle);
	curl_slist_free_all(headers);
	handle = NULL;
	headers = NULL;
}
