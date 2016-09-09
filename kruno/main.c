#include "headers/daemon.h"
#include <curl/curl.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void do_jsmin(FILE *file, FILE *file_min)
{
	char command[100];
	for (int i=0; i<100; i++) command[i] = '\0';
	snprintf(command, 100, "sh jsmin<&%i>&%i\n", 
				fileno(file), fileno(file_min));
	for (int i=0; i<100; i++) printf("%c", command[i]);
	system(command); 
}


int main()
{
	#ifdef DAEMON
		daemon_mk(NULL, "/var/log/warden.log",
		"/var/log/warden.err.log");				
	#endif

	#ifdef CURLING
		curl_global_init(CURL_GLOBAL_ALL);
		CURL *store_curl_handle;
		#define CURL_LOG_PATH "/git/Project-Warden/Axolotl/kruno/"
		FILE *store_curl_data_file = fopen(
			CURL_LOG_PATH "store_curl_response.log", "w+");
		FILE *store_curl_data_file_min = fopen(
			CURL_LOG_PATH "store_curl_response.min.log", "w+");
		FILE *store_curl_header_file = fopen(
			CURL_LOG_PATH "store_curl_headers.log", "w+");
/*		setup_store_curl_handle(store_curl_handle,
					store_curl_data_file, 
					store_curl_header_file);*/

		fprintf(store_curl_data_file, "{\n   \"index : value\"\n}");
		do_jsmin(store_curl_data_file, store_curl_data_file_min);
	#endif
/*	
	for (int i=0; i<3; i++)	
	{
		printf("test\n");
		#ifdef CURLING
		#endif
		sleep(5);
	}
	daemon_end(); */
	return 0;
}


