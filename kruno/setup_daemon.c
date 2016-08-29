#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include <stdarg.h>
#include <stdbool.h>


/*
 * exit_fail_with_errno - prints the given errno value's associated string to
 * stderr and exits with fail.
 */
void exit_fail_with_errno(int errno_in)
{
	fprintf(stdout, "errno= %d: %s\n", errno_in, strerror(errno_in) );
	exit(EXIT_FAILURE);
}

/*
 * redirect_oe_streams - redirects stderr and stdout when specified by
 * parameter.
 *
 * @notes: currently takes string for redirect file path, re-write to take FILE
 * pointer?
 */
void redirect_oe_streams(const char *indicator_string, va_list ap)
{
	#define NUM_FDS 3
	/* index by values -> [STDIN_FILENO],[STDOUT_FILENO],[STDERR_FILENO] */
	bool stream_was_redirected[NUM_FDS] = {false, false, false};	
	if (indicator_string != NULL) {

		/* redirect streams so the calling argument specifications... */
		for (; *indicator_string != '\0'; indicator_string++) {
	printf("LINE 42 - for loop...\n");
			if (*indicator_string == 'e') {
	printf("LINE 43 - for loop...\n");
				FILE *stderr_file = freopen(
					va_arg(ap, const char *), "a", stderr);
	printf("LINE 46 - for loop...\n");

				if (stderr_file == NULL) {
					exit_fail_with_errno(errno);
				}
				fprintf(stderr, "to error file \n");
				stream_was_redirected[STDERR_FILENO] = true;

			} else if (*indicator_string == 'o') {
	printf("LINE 55 - for loop...\n");
				FILE *stdout_file  = freopen(
					va_arg(ap, const char *), "a", stdout);

	printf("LINE 59 - for loop...\n");
				if (stdout_file == NULL) {
					exit_fail_with_errno(errno);
				}
				printf("to output file\n");
				stream_was_redirected[STDOUT_FILENO] = true;
			}
		}
	}

	/* close the standard I/O streams that have not been redireceted... */
	for (int i=0; i<NUM_FDS; i++) {
		if (stream_was_redirected[i] == false) {
			close(i);
		}
	}
	#undef NUM_FDS
}

void setup_daemon(const char *indicator_string, ...)
{
	printf("LINE 75\n");
	
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
	printf("LINE 99\n");
	/*
	 * it's advisable to keep a log file for debugging at least.  I should figure out how to
	 * make one...  In order to access files cretated by the daemon, we need to unmask them.
	 */
	umask(0);
	printf("LINE 105\n");

	/* Creating an SID is very similar to creating a fork */
	sid = setsid();
	if (sid<0)
	{
		/* you should log any errors right here... */
		exit(EXIT_FAILURE);
	}
	printf("LINE 114\n");

	/*
	 * fork again (per stack overflow, but not per devin...)
	 */
	pid = fork();
	printf("LINE 121\n");
	if (pid<0)
	{
		printf("LINE 123\n");
		exit(EXIT_FAILURE);
	}

	if (pid>0)
	{
		printf("LINE 130\n");
		exit(EXIT_SUCCESS);
	}
	printf("LINE 133\n");
	

	/*
	 * It's very important to change the current working directory to something that will
	 * always be there.  Not all linux distros completely follow the Linux Filesystem
	 * Hierarchy standard, the ONLY gauranteed directory is root (/).
	 */
	if ((chdir("/"))<0)	// chdir() returns -1 on failure...
	{
		/* log any failure here... */
		exit(EXIT_FAILURE);
	}

	/* redirect stderr and stdout per arguments */
	va_list ap;
	printf("LINE 81\n");
	va_start(ap, indicator_string);
	redirect_oe_streams(indicator_string, ap);
	va_end(ap);

	/* 
	 * NOTE: man close said errors should be checked here even though most prople don't.  I'm
	 * not checking any because we havent used any of these files...
	 */

	/* Daemon-specific initializations go here... */

	/*
	 * the big daemon infinite loop (technically not "infinite")
	 */
	exit(EXIT_SUCCESS);
}

int main()
{
	setup_daemon("eo", "errfile.txt", "outfile.txt");
	return 0;
}
