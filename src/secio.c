#include "secio.h"

#include "fcgi_stdio.h"

#include <stdio.h>
#include <unistd.h>

void
print_s(const char * const buf)
{
	int i = 0;
	while (buf[i] > 0 && buf[i] <= '~')
	{
		fwrite((void *)&buf[i++], sizeof(char), 1, FCGI_stdout);
	}
}
