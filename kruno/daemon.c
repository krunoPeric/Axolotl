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
	 * It's very important to change the current working directory to something that will
	 * always be there.  Not all linux distros completely follow the Linux Filesystem
	 * Hierarchy standard, the ONLY gauranteed directory is root (/).
	 */
	if ((chdir("./"))<0)	// chdir() returns -1 on failure...
	{
		/* log any failure here... */
		exit(EXIT_FAILURE);
	}
	
	/*
	 * In order to access files cretated by the daemon, we need to unmask them.
	 */
	umask(0);

	/*
	 * File Descriptors are unneeccssary for daemons because they cannot use the terminal.  We
	 * close them out because they are redundant and a potential securty hazard.
	 */
	//close(STDIN_FILENO);		// (0)
	//close(STDOUT_FILENO);		// (1)
	//close(STDERR_FILENO);		// (2)
	
	freopen("stderr.txt", "a", fdopen(STDERR_FILENO, "a"));
	freopen("stdout.txt", "a", fdopen(STDOUT_FILENO, "a"));

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
		fprintf(stdout_new, "running daemon stdout...\n");
		sleep(7);	// wait 30 seconds
	}
	exit(EXIT_SUCCESS);
}