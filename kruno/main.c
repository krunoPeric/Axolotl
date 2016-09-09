#include "headers/daemon.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	#ifdef DAEMON
		daemon_mk(NULL, "/var/log/warden.log",
		"/var/log/warden.err.log");				
	#endif

	/*
	 * working with jsmin system commands.  Here the file use to log the
	 * jsmin outputs will be appeneded.  Plan on having it overwrite a temp
	 * file everytime for "final" daemon commit....
	 */

	#define SYS_COMMAND \
	"jsmin < /git/Project-Warden/Axolotl/kruno/sample_js.js \
	>> /git/Project-Warden/Axolotl/kruno/jsmin_test.log"

	FILE *jsmin_logger = fopen(
	"/git/Project-Warden/Axolotl/kruno/jsmin_test.log", "a");
	
	for (int i=0; i<3; i++)	
	{
		printf("test\n");
		system(SYS_COMMAND);
		fprintf(jsmin_logger, "\n");
		fflush(jsmin_logger);
		sleep(5);
	}
	daemon_end();
}
