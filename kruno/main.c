#include "headers/daemon.h"

#include <unistd.h>
#include <stdio.h>

int main()
{
	#ifdef DAEMON
		daemon_mk(NULL, "/var/log/warden.log",
		"/var/log/warden.err.log");				
	#endif

	while (1)
	{
		printf("test\n");
		sleep(5);
	}

	daemon_end();
}
