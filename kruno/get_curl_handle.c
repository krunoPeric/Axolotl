#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

void setup_store_curl_handle(CURL *curl_handle, FILE *curl_data_file,
						FILE *curl_header_file)
{
	CURLcode res;
	char *url = "127.0.0.1:1991/places/store/iface/light";
	curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_URL, url); 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)curl_data_file); 
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
	/* get the response... */
	res = curl_easy_perform(curl_handle);
	if(res != CURLE_OK)
	{
		printf("curl_easy_perform() falied %s\n",
			curl_easy_strerror(res));
	}
	else
	{
		/* print the reponse */
		long size_of_chunk = (long)chunk.size;
		printf("%lu bytes retrieved\n", size_of_chunk);
		for (long i=0; i<size_of_chunk; i++)
		{
			printf("%c", chunk.memory[i]);
		}
		/* print the headers */
		long size_of_header_chunk = (long)header_chunk.size;
		printf("%lu bytes retrieved\n", size_of_header_chunk);
		for (long i=0; i<size_of_header_chunk; i++)
		{
			printf("%c", header_chunk.memory[i]);
		}
		/* setup info extraction for last recieved HTTP response... */
		long res_code;
		curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &res_code);
		curl_easy_cleanup(curl_handle);
		printf("\nResponse Code Extracted: %lu\n", res_code);
	}
	free(chunk.memory);
	free(header_chunk.memory);
	curl_global_cleanup();
	return 0;
}
