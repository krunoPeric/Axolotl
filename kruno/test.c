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
	fprintf(stderr, "errno= %d: %s\n", errno_in, strerror(errno_in) );
	exit(EXIT_FAILURE);
}

/*
 * redirect_oe_streams - redirects stderr and stdout when specified by
 * parameter.
 *
 * @notes: currently takes string for redirect file path, re-write to take FILE
 * pointer?
 */
void redirect_oe_streams(const char *indicator_string, ...)
{
	#define NUM_FDS 3
	/* index by values -> [STDIN_FILENO],[STDOUT_FILENO],[STDERR_FILENO] */
	bool stream_was_redirected[NUM_FDS] = {false, false, false};	
	if (indicator_string != NULL) {
		va_list ap;
		va_start(ap, indicator_string);

		/* redirect streams so the calling argument specifications... */
		for (; *indicator_string != '\0'; indicator_string++) {
			if (*indicator_string == 'e') {
				FILE *stderr_file = freopen(
					va_arg(ap, const char *), "a", stderr);

				if (stderr_file == NULL) {
					exit_fail_with_errno(errno);
				}
				fprintf(stderr, "to error file \n");
				stream_was_redirected[STDERR_FILENO] = true;

			} else if (*indicator_string == 'o') {
				FILE *stdout_file  = freopen(
					va_arg(ap, const char *), "a", stdout);

				if (stdout_file == NULL) {
					exit_fail_with_errno(errno);
				}
				printf("to output file\n");
				stream_was_redirected[STDOUT_FILENO] = true;
			}
		}
		va_end(ap);
	}

	/* close the standard I/O streams that have not been redireceted... */
	for (int i=0; i<NUM_FDS; i++) {
		if (stream_was_redirected[i] == false) {
			close(i);
		}
	}
	#undef NUM_FDS
}

int main(void)
{
	redirect_oe_streams("eo", "errfile.txt", "outfile.txt");
	printf("test test\n");
	fprintf(stderr, "error test test\n");
	return 0;
}
