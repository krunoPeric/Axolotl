#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

#define CLEAR_LOGS \
	freopen(CURL_LOG_PATH "store_curl_response.log", "w+",\
					store_curl_data_file);\
	freopen(CURL_LOG_PATH "store_curl_response.min.log", "w+",\
					store_curl_data_file_min);\
	freopen(CURL_LOG_PATH "store_curl_headers.log", "w+",\
					store_curl_header_file);

#define FLUSH_LOGS \
	fflush(store_curl_data_file);\
	fflush(store_curl_header_file);

void setup_store_curl_handle(CURL *curl_handle, FILE *curl_data_file,
				FILE *curl_header_file);

void do_curl(CURL *curl_handle);

void do_jsmin(const char *file, const char *file_min);
