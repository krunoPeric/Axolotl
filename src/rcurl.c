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
#include "termio.h"

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define CURL_FAILURE 0
#define CURL_SUCCESS 1

struct rcurl_fetch_bus
{
	char *payload;
	size_t size;
};

static CURL *handle;
static struct curl_slist *headers;

size_t _callback(void *chunk, size_t size, size_t nmemb, void *umemp)
{
	size_t sizeb = size * nmemb;
	struct rcurl_fetch_bus *bus = (struct rcurl_fetch_bus *) umemp;

	// Resize buffer
	bus->payload = (char *) realloc(bus->payload, bus->size + sizeb + 1);

	// Check buffer
	if (bus->payload == NULL)
	{
		destroy(bus->payload);

		return -1;
	}

	// Copy to buffer
	memcpy(&(bus->payload[bus->size]), chunk, sizeb);
	bus->size += sizeb;
	bus->payload[bus->size] = '\0';

	return sizeb;
}

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
	curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(handle, CURLOPT_USERAGENT, "axolotol-agent/1.0");
	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(handle, CURLOPT_TIMEOUT, 5);

	// Set callback function for writing
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, _callback);

	return CURL_SUCCESS;
}

void rcurl_destroy()
{
	curl_easy_cleanup(handle);
	curl_slist_free_all(headers);
	handle = NULL;
	headers = NULL;
}

char *rcurl_fetch(const char * const url)
{
	CURLcode rcode;
	struct rcurl_fetch_bus bus;

	// Initiralize bus
	bus.payload = (char *) calloc(1, sizeof(char));
	if (bus.payload == NULL)
	{
		return NULL;
	}

	bus.size = 0;

	// Set URL
	curl_easy_setopt(handle, CURLOPT_URL, url);

	// Pass struct address to cURL for writing
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, ((void *) &bus));

	// Fetch URL
	rcode = curl_easy_perform(handle);
	if (rcode != CURLE_OK || bus.size < 1)
	{
		return NULL;
	}
	else
	{
		return bus.payload;
	}
}
