#include "serialize.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

char * __attribute__((const))
_pair(const char * const key, const char * const value);

char * __attribute__((const))
_pair_s(const char *key, const char *value);

char *
_pair_s(const char *key, const char *value)
{
	char *result;
	const char *key_s = key;
	const char *value_s = value;

	if (key == NULL)
		key_s = "null";
	
	if (value == NULL)
		value_s = "null";

	result = _pair(key_s, value_s);
	
	return result;
}

char *
_pair(const char * const key, const char * const value)
{
	char *result;
	size_t len_k = strlen(key);
	size_t len_v = strlen(value);
	size_t len_r = len_k + len_v + 7;

	if (len_k == 0 || len_v == 0)
	{
		return NULL;
	}

	result = malloc(len_r*sizeof(char));

	// Create key block
	strcpy(result, "\"");
	strcat(result, key);
	strcat(result, "\": \"");
	
	// Create value block
	strcat(result, value);
	strcat(result, "\"");

	return result;
}

char *
serialize(const char * const key, const char * const value)
{
	char *result = NULL;
	char *keypair = _pair_s(key, value);
	size_t len_r;

	if (keypair != NULL) {
		len_r = strlen(keypair);
		len_r += 3;

		result = malloc(len_r*sizeof(char));
		strcpy(result, "{");
		strcat(result, keypair);
		strcat(result, "}");

		free(keypair);
	}

	return result;
}
