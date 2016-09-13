#ifdef JSMN
	#include "include/jsmn_functions.h"
#endif

#ifdef DAEMON
	#include "include/daemon.h"
#endif

#ifdef CURLING
	#include "include/curl_functions.h"
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

		memory_struct *response_chunk = malloc(sizeof(memory_struct));
		memory_struct *header_chunk = malloc(sizeof(memory_struct));
		response_chunk->memory = malloc(1);
		response_chunk->size = 0;
		header_chunk->memory = malloc(1);
		header_chunk->size = 0;

		setup_store_curl_handle(store_curl_handle, response_chunk, header_chunk);
	#endif

	#ifdef JSMN
		light_t *light_data;
	#endif

//	while(1)	
	for (int i=0; i<1; i++)
	{
		printf("test\n");
		#ifdef CURLING
			do_curl(store_curl_handle);

			print_chunk(header_chunk);
			print_chunk(response_chunk);
		#endif

		#ifdef JSMN
		printf("test line 59\n");
			parse_json_to_light_t(response_chunk->memory,
								light_data);
			for (int i=0; i<13; i++){ printf(light_data[i].date); printf("test\n");}
		#endif


//		sleep(5);
	}

	#ifdef DAEMON
		daemon_end();	/* close standard file descriptors */
	#endif

	#ifdef CURLING
		free(response_chunk->memory);
		free(header_chunk->memory);
		free(response_chunk);
		free(header_chunk);

		curl_easy_cleanup(store_curl_handle);
		curl_global_cleanup();
	#endif
	return 0;
}
