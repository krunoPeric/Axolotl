#include "../include/curl_functions.h"
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>


size_t write_callback(void *contents, size_t size, size_t nmemb, void *userdata)
{
	/* size of data = size*nmemb.  Data passed is not null terminated! */
	size_t real_size = size*nmemb;
	struct memory_struct *memory_block = (struct memory_struct *)userdata;
	memory_block->memory = realloc(memory_block->memory, 
					memory_block->size + real_size + 1);

	if (memory_block->memory == NULL)
	{
		printf("not enough memory (ralloc returned NULL)\n");
		return 0;
	}

	/*
	 * copy the recieved curl data to the memory component of the
	 * memory_struct pointer passed by userdata...
	 */
	memcpy(&(memory_block->memory[memory_block->size]), contents, real_size);
	/* update the size of the memory chunk... */
	memory_block->size += real_size;
	memory_block->memory[memory_block->size] = 0;
	return real_size;
}



void setup_store_curl_handle(CURL *curl_handle, memory_struct *response_chunk, memory_struct *header_chunk)
{
	char *url =
	"http://api.osudev.club/places/store/light/since/2011-12-24T12:12:12.100Z";
//	"127.0.0.1:1991/places/store/light/since/2011-12-24T12:12:12.100Z";
	curl_easy_setopt(curl_handle, CURLOPT_URL, url); 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)response_chunk); 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, 
				(void *)header_chunk);
	curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, write_callback);

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

void print_chunk(memory_struct *chunk)
{
	/* print the reponse */
	long size_of_chunk = (long)chunk->size;
	printf("%lu bytes retrieved\n", size_of_chunk);
	for (long i=0; i<size_of_chunk; i++)
	{
		printf("%c", chunk->memory[i]);
	}
}
