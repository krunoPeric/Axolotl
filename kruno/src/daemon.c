#include "../include/daemon.h"

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

void handle_daemon_error(int errno_in)
{
}

void daemon_mk(const char *stdin_file, const char *stdout_file, const char
*stderr_file)
{
	if (stdin_file == NULL){
		stdin_file = "/dev/null";
	}
	if (stdout_file == NULL){
		stdout_file = "/dev/null";
	}
	if (stderr_file == NULL){
		stderr_file = "/dev/null";
	}

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

	if ((chdir("/"))<0)
	{
		exit(EXIT_FAILURE);
	}
	
	umask(0);

	fclose(stdin); fclose(stdout); fclose(stderr);

	freopen(stdin_file, "r", stdin);
	freopen(stdout_file, "a", stdout);
	freopen(stderr_file, "a", stderr);

	setbuf(stdout, NULL);	setbuf(stderr, NULL);
}

void daemon_end()
{
	fclose(stdin); fclose(stdout); fclose(stderr);
}
