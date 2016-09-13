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
	      (int) strlen(comparison_string) == token_length &&
	      strncmp(json + token->start, comparison_string, token_length)  )
	{
		return 0;
	}
	return -1;
}

int parse_json_to_light_t(const char *json, light_t *light_data_set)
{
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
		if (token_equals(json, &tokens[i], "error") == 0) {
			if (json[tokens[i+1].start] == 't')
				;				/* TODO? */
			else
				;				/* TODO? */
			i++;
			printf("test61\n");
		}
		else if (token_equals(json, &tokens[i], "payload") == 0) {
			i++;
			if (tokens[i].type == JSMN_OBJECT)
			{
				int num_children = tokens[i].size;
				for (int j=0; j<num_children; j++)
				{
					i++;	/* move on to "dates" (expected...) */
					if (token_equals(json, &tokens[i], "dates") == 0) {
						i++;	/* move on to array of dates (expected...) */	
						if (tokens[i].type == JSMN_ARRAY)
						{
							/* allocate memory for the array of structs... */
							int num_elems = tokens[i].size;
							light_data_set = malloc(sizeof(light_t) * num_elems);	
							i++;	/* move on to first date (expected...) */
							for (int k=0; i < (i+num_elems-1); i++, k++) {
								int size_of_date_string = tokens[i].end - tokens[i].start;	
								light_data_set[k].date = malloc(size_of_date_string);
							}
						}
					}
				}
			}
		}
	}
	return 0;
}
