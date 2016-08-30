#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>

int main(void)
{
	pid_t pid, sid;
	
	pid = fork();

	if (pid<0)
	{
		exit(EXIT_FAILURE);
	}

	if (pid>0)
	{
		exit(EXIT_SUCCESS);
	}
	
	sid = setsid();
	if (sid<0)
	{
		exit(EXIT_FAILURE);
	}

	pid = fork();
	if (pid<0)
	{
		exit(EXIT_FAILURE);
	}

	if (pid>0)
	{
		exit(EXIT_SUCCESS);
	}

	if ((chdir("/git/Project-Warden/Axolotl/kruno"))<0)
	{
		exit(EXIT_FAILURE);
	}
	
	umask(0);


	fclose(stdin); fclose(stdout); fclose(stderr);

	freopen("warden.log", "a", stdout);

		

	while(1)
	{
		printf("running daemon...\n");
		fflush(stdout);
		sleep(5);
	}
	exit(EXIT_SUCCESS);
}
