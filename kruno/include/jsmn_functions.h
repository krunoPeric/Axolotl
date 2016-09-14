#ifndef JSMN_FUNCTIONS
#define JSMN_FUNCTIONS

#include <stdint.h>

#include "../../jsmn/jsmn.h"
#include "../include/jsmn_functions.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct light_t
{
	char *date;
	int *state;
} light_t;


int token_equals(const char *json, jsmntok_t *token, 
					const char *comparison_string);

int parse_json_to_light_t(const char *json, light_t ***light_data);

#endif
