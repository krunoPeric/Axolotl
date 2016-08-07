#include "serialize.h"

#include <stdio.h>
#include <string.h>

int main()
{
	char *result = serialize("error", "reason");
	printf("%s\n", result);
	free(result);

	char *test = "ABC";
	char *asd = malloc(4*sizeof(char));
	strcpy(asd, test);
	asd[3] = 'd';
	//test[3] = 'D';
	int i = 0;
	
	while (asd[i] <= '~' && asd[i] > 0)
	{
		write(1, &asd[i++], sizeof(char));
	}

	return 0;
}
