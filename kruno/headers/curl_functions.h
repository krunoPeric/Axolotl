#ifndef CURL_FUNCTIONS_H
#define CURL_FUNCTIONS_H

#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

typedef struct memory_struct
{
	char *memory;
	size_t size;
} memory_struct;

size_t write_callback(void *contents, size_t size, 
				size_t nmemb, void *userdata);
void setup_store_curl_handle(CURL *curl_handle, memory_struct *chunk, 
					memory_struct *header_chunk);
void do_curl(CURL *curl_handle);
void print_chunk(memory_struct *chunk);

#endif
