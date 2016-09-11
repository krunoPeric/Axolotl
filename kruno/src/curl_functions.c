#include "../headers/curl_functions.h"
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

void setup_store_curl_handle(CURL *curl_handle, FILE *curl_data_file,
				FILE *curl_header_file)
{
	char *url = "127.0.0.1:1991/places/store/iface/light";
	curl_easy_setopt(curl_handle, CURLOPT_URL, url); 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA,
			(void *)curl_data_file); 
	curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, 
			(void *)curl_header_file);

	/* 
	 * some servers don't like requests that are made without a user-agent
	 * field.   One is provided just in case...
	 */
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	return;
}

void do_curl(CURL *curl_handle)
{
	CURLcode res;
	/* get the response... */
	res = curl_easy_perform(curl_handle);
	if(res != CURLE_OK)
	{
		printf("curl_easy_perform() falied %s\n",
			curl_easy_strerror(res));
	}
	else
	{
		/* setup info extraction for last recieved HTTP response... */
		long res_code;
		curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &res_code);
		printf("\nResponse Code Extracted: %lu\n", res_code);
	}
	return;
}

void do_jsmin(const char *file, const char *file_min)
{
	/*
	 * TODO:
	 *	figure out how to do this using file descriptors instaed.  Make
	 *	a bash script and then run it with system?
	 *	system using /bin/sh instead of bin/bash...doesn't like using
	 *	file descriptors...
	 */
	const int len = 300;
	char command[len];
	for (int i=0; i<len; i++) command[i] = '\0';
	snprintf(command, len, "jsmin < %s > %s\n", file, file_min);
	system(command); 
}

