#include "../../jsmn/jsmn.h"
#include "../include/jsmn_functions.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int token_equals(const char *json, jsmntok_t *token, 
					const char *comparison_string)
{
	printf("test ttoken_equals\n");
	int token_length = token->end - token->start;

	/* 
	 * Check to see if the string indexed by the token object is equivalent
	 * to comparison_string
	 */
	if (  token->type == JSMN_STRING &&
	      (int)strlen(comparison_string) == token_length &&
	      strncmp(json + token->start, comparison_string, token_length) == 0  )
	{
		return 0;
	}
	return -1;
}

void print_token(const char *json, jsmntok_t *token, int token_index)
{
	printf("token #%i: , token.start = %i, token.end = %i  ", token_index,
							token->start, token->end);

	printf("%.*s  ", token->end - token->start, json + token->start);

	printf("\n");


}

int parse_json_to_light_t(const char *json, light_t ***light_data_set)
{
	int num_elems;
	printf("test\n");
	jsmn_parser parser;
	jsmn_init(&parser);
	/* get the required number of tokens by arguing NULL... */
	int num_tokens = jsmn_parse(&parser, json, strlen(json),
		NULL, 0);
	printf("test\n");
	/* reset the parser... */
	jsmn_init(&parser);
	jsmntok_t tokens[num_tokens];
	int parse_result = jsmn_parse(&parser, json, strlen(json), 
							tokens, num_tokens);
	
	printf("test\n");
	if (parse_result < 0) {
		printf("Failed to parse JSON: %d\n", parse_result);
		/* exit now ?? */
	}
	/* Check that top level token is an object... */	
	if (parse_result < 1 || tokens[0].type != JSMN_OBJECT) {
		printf("Not JSON\n");
		/* exit now?? */
	}
	printf("test 51 jsmn\n");
	/* process jsmn parse results... */



	
	for (int i=0; i<parse_result; i++) {
		printf("i = %i ", i);
		printf("\033[32m");
		print_token(json, &tokens[i], i);
		printf("\033[31m");
		if (token_equals(json, &tokens[i], "error") == 0) {
			if (json[tokens[i+1].start] == 'f')
				printf("error = false\n");
								/* TODO? */
			else
				printf("error = true\n");
								/* TODO? */
			i++;
		}
		else if (token_equals(json, &tokens[i], "payload") == 0) {
			printf("payload\n");
			i++;
			if (tokens[i].type == JSMN_OBJECT)
			{
				int num_children = tokens[i].size;
				for (int j=0; j<num_children; j++)
				{
					i++;	/* move on to "dates" (expected...) */
					if (token_equals(json, &tokens[i], "dates") == 0) {
						printf("dates\n");
						i++;	/* move on to array of dates (expected...) */	
						if (tokens[i].type == JSMN_ARRAY)
						{
							printf("jsmn_array\n");
							/* allocate memory for the array of structs... */
							num_elems = tokens[i].size;
							printf("number of dates = %i\n", num_elems);
							printf("alloc array...\n");
							*light_data_set =
								malloc(sizeof(light_t *) * num_elems);	
							i++;	/* move on to first date (expected...) */
#define skip
#ifndef skip
#endif
							for (int k=0; k < num_elems; k++) {
								printf("i = %i, alloc struct element...\n", i);
								(*light_data_set)[k] = malloc(sizeof(light_t));
								(*light_data_set)[k]->date = malloc(sizeof(char) * 11);
								strncpy((*light_data_set)[k]->date, json+tokens[i].start, 11);
								(*light_data_set)[k]->date[10] = '\0';
								printf("\033[33mdate[%i] = %s\n", k, (*light_data_set)[k]->date);
								printf("\033[32m");
								print_token(json, &tokens[i], i);
								printf("\033[31m");
								i++;
							}
						}
					}
				}
			}
		}
	}
	printf("\033[0m");
	return 0;
}
