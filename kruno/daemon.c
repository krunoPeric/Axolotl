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


	close(STDIN_FILENO);close(STDOUT_FILENO);close(STDERR_FILENO);
	int warden_log = open("warden.log", O_RDWR|O_CREAT|O_APPEND,
					    S_IRUSR|S_IWUSR|S_IRGRP); 
	dup2(warden_log, STDOUT_FILENO);
	dup2(warden_log, STDERR_FILENO);

	while(1)
	{
		printf("running daemon...\n");
		fflush(stdout);
		sleep(5);
	}
	exit(EXIT_SUCCESS);
}
