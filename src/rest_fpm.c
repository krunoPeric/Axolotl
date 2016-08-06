#include "rest_fpm.h"

#include "printutils.h"

#include <fcgi_stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define destroy(P) {\
			free(P);\
			P = NULL;\
		}

#define MAX_FCGI_PARAMS 17
#define MAX_QUERIES 11

#define DATABASE_URL "https://script.google.com/macros/s/ABC123/exec?place="

const char * const FCGI_PARAMS[MAX_FCGI_PARAMS] =
{
	"GATEWAY_INTERFACE",
	"SERVER_SOFTWARE",
	"QUERY_STRING",
	"REQUEST_METHOD",
	"CONTENT_TYPE",
	"CONTENT_LENGTH",
	"SCRPIE_FILENAME",
	"SCRIPT_NAME",
	"REQUEST_URI",
	"DOCUMENT_URI",
	"DOCUMENT_ROOT",
	"SERVER_PROTOCOL",
	"REMOTE_ADDR",
	"REMOTE_PORT",
	"SERVER_ADDR",
	"SERVER_PORT",
	"SERVER_NAME"
};

struct rest_query
{
	size_t count;
	size_t size;
	char **query;
};

void init_curl(int *status, CURLM *cmh, CURL* (*ch)[REST_MAX_T], const int max)
{/*
  int retval;
  CURLMsg *msg;

  for (i = 0; i < max; i++)
  {
   *ch[i] = curl_easy_init();
  }

  curl_easy_setopt(*ch[0]), CURLOPT_URL, "http://example.com");

  multi_handle = curl_multi_init();

  for (i = 0; i < max; i++)
  {
    curl_multi_add_handle(*cmh, *ch[i]);
  }*/

}

struct rest_query *
explode(const char *query)
{
	struct rest_query *rq;
	size_t i;
	size_t size;
	size_t buffpos;

	if (query == NULL)
		return NULL;

	rq = malloc(sizeof(struct rest_query));
	rq->query = malloc(11*sizeof(char *));
	rq->count = 0;
	rq->size = 0;

	i = 0;
	while (query[i++] != '\0');

	size = i;
	if (size <= 1)
		return NULL;

	buffpos = 1;	

	for (i = 1; i < size; i++)
	{	
		if (query[i] == '/' || query[i] == '\0')
		{
			size_t pos = rq->count;
			size_t len = (i - buffpos);

			if (pos >= 11 || len <= 0)
				break;
			
			rq->size += len;

			rq->query[pos] = malloc((len+1)*sizeof(char));
			rq->query[pos] = memcpy(rq->query[pos], &(query[buffpos]), len);
			rq->query[pos][len] = '\0';

			buffpos = i+1;
			++(rq->count);
		}
	}
	
	return rq;
}

char* itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36)
	{
		*result = '\0';
		return result;
	}

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while (value);

	// Apply negative sign
	if (tmp_value < 0)
		*ptr++ = '-';
	
	*ptr-- = '\0';
	
	while (ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}

	return result;
}

char *post_serialize(struct rest_query *rq)
{
	char *json;
	char index[3];
	size_t len;
	size_t i;

	if (rq == NULL)
		return NULL;

	if (rq->count <= 1)
		return NULL;

	len = sizeof(DATABASE_URL) - 6 + rq->size + 3*rq->count + 1;
	json = malloc(len*sizeof(char));

	if (strcmp(rq->query[0], "places") != 0)
		return NULL;

	strcpy(json, DATABASE_URL);
	strcat(json, rq->query[1]);
	
	i = 2;
	while (i < rq->count)
	{
		strcat(json, "&");
		itoa(i-2, index, 10);
		strcat(json, index);
		strcat(json, "=");
		strcat(json, rq->query[i]);

		++i;
	}

	return json;
}

int
main(int argc, char *argv[])
{
	//CURL *curl_handles[REST_MAX_T];
	//CURLM *curl_multi_handle;

	int exit_status = EXIT_SUCCESS;
	//const int max_requests = REST_MAX_REQUESTS;
	
	char *request_uri;
	char *request_json;
	struct rest_query *request;
	int i;
	
	#ifndef __GNUC__
		return EXIT_FAILURE;
	#endif

	while (FCGI_Accept() >= 0)
	{
		printf("Status: 250\r\n");
		printf("Content-type: application/json\r\n");
		printf("\r\n");

		request_uri = getenv("REQUEST_URI");
		if (request_uri == NULL)
		{
			// TODO Log Exception
			printf("{\"error\": \"could not get environment variable\"}");
			continue;
		}
		
		request = explode(request_uri);
		if (request == NULL)
		{
			// TODO Log exception
			printf("{\"error:\" \"invalid request uri\"}");
			continue;
		}

		request_json = post_serialize(request);
		if (request_json == NULL)
		{
			printf("{\"error\": \"could not serialize request\"}");
		}
		else
		{
			printf("{\"database_uri\": \"%s\"}", request_json);
			destroy(request_json);
		}

		for (i = 0; i < request->count; i++)
			destroy(request->query[i]);
		destroy(request->query);
		destroy(request);
	}

	//init_curl(&exit_status, &curl_multi_handle, &curl_handles, REST_MAX_T);

	return exit_status;
}
