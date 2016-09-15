#include "include/jsmn_functions.h"

#include "include/daemon.h"

#include "include/curl_functions.h"
#include <curl/curl.h>


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
		/* 
		 * for array of light_t structs generated from parsing the store
		 * GET response 
		 */
		light_t **light_data = NULL;
		int num_states = 0;
	#endif

	while(1)	
	{
		light_data = NULL;
		#ifdef CURLING
			do_curl(store_curl_handle, response_chunk, header_chunk);

			print_chunk(header_chunk);
			print_chunk(response_chunk);
		#endif

		#ifdef JSMN
			num_states = parse_json_to_light_t(response_chunk->memory,
								&light_data);

			/* print the array of light_t structs... */
			printf("\nThe array of light_t structs parsed from json:\n");
			for (int i=0; i<num_states; i++) 
				printf("light_t_data[%i]: date = %s, state = %i\n", i, light_data[i]->date, light_data[i]->state);

			/* free the array of light_t for next call... */
			for (int i=0; i<num_states; i++) free(light_data[i]->date);
			for (int i=0; i<num_states; i++) free(light_data[i]);
			free(light_data);

		#endif


		sleep(60);
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
