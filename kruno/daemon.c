#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

int main(void)
{

	pid_t pid, sid;
	/* Fork off the parent process */
	pid = fork();	// this return the process id
	/*
	 * A good pid must a value greater than zero.  If it's good, the program exists with
	 * EXIT_SUCCESS, which allows the process to continue from here on in the code.
	 */
	if (pid<0)
	{
		exit(EXIT_FAILURE);
	}

	if (pid>0)
	{
		exit(EXIT_SUCCESS);
	}
	
	/* Here's the advisable place to open any log files.... */

	/* Creating an SID is very similar to creating a fork */
	sid = setsid();
	if (sid<0)
	{
		/* you should log any errors right here... */
		exit(EXIT_FAILURE);
	}

	/*
	 * fork again (per stack overflow, but not per devin...)
	 */
	pid = fork();
	if (pid<0)
	{
		exit(EXIT_FAILURE);
	}

	if (pid>0)
	{
		exit(EXIT_SUCCESS);
	}
	
	/*
	 * In order to access files cretated by the daemon, we need to unmask them.
	 */
	umask(0);

	/*
	 * It's very important to change the current working directory to something that will
	 * always be there.  Not all linux distros completely follow the Linux Filesystem
	 * Hierarchy standard, the ONLY gauranteed directory is root (/).
	 */
	if ((chdir("/git/Project-Warden/Axolotl/kruno"))<0)	// chdir() returns -1 on failure...
	{
		/* log any failure here... */
		exit(EXIT_FAILURE);
	}
	

	/*
	 * File Descriptors are unneeccssary for daemons because they cannot use the terminal.  We
	 * close them out because they are redundant and a potential securty hazard.
	 */

	int warden_FD = open("warden.log", O_RDWR|O_APPEND|O_CREAT,
						     S_IWUSR|S_IRUSR|S_IRGRP);

	dup2(warden_FD, STDERR_FILENO);
	dup2(warden_FD, STDOUT_FILENO);
	

	/* 
	 * NOTE: man close said errors should be checked here even though most prople don't.  I'm
	 * not checking any because we havent used any of these files...
	 */

	/* Daemon-specific initializations go here... */

	/*
	 * the big daemon infinite loop (technically not "infinite")
	 */
	while (1)
	{
		/* do some tasks here... */
		fprintf(stderr, "running daemon stderr...\n");
		fprintf(stdout, "running daemon stdout...\n");
		sleep(7);	// wait 30 seconds
	}
	exit(EXIT_SUCCESS);
}
