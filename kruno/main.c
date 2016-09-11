#ifdef DAEMON
#include "headers/daemon.h"
#endif

#ifdef CURLING
#include "headers/curl_functions.h"
#include <curl/curl.h>
#endif


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
	#ifdef DAEMON
		daemon_mk(NULL, "/var/log/warden.log",
		"/var/log/warden.err.log");				
	#endif

	#ifdef CURLING
		curl_global_init(CURL_GLOBAL_ALL);
		CURL *store_curl_handle;
		store_curl_handle = curl_easy_init();
		#define CURL_LOG_PATH "/git/Project-Warden/Axolotl/kruno/"
		FILE *store_curl_data_file = fopen(
			CURL_LOG_PATH "store_curl_response.log", "w+");
		FILE *store_curl_data_file_min = fopen(
			CURL_LOG_PATH "store_curl_response.min.log", "w+");
		FILE *store_curl_header_file = fopen(
			CURL_LOG_PATH "store_curl_headers.log", "w+");
		setup_store_curl_handle(store_curl_handle,
					store_curl_data_file, 
					store_curl_header_file);
	#endif

	while(1)	
	{
		printf("test\n");
		#ifdef CURLING
			CLEAR_LOGS
			do_curl(store_curl_handle);
			FLUSH_LOGS
			do_jsmin(CURL_LOG_PATH "store_curl_response.log", 
				 CURL_LOG_PATH "store_curl_response.min.log");
		#endif
		sleep(5);
	}
	daemon_end(); 

	#ifdef CURLING
		curl_easy_cleanup(store_curl_handle);
		curl_global_cleanup();
	#endif
	return 0;
}
