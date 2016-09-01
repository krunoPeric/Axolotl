#ifndef DAEMON_H
#define DAEMON_H

void handle_daemon_error(int errno_in);

void daemon_mk(const char *stdin_file, const char *stdout_file, const char
*stderr_file);

void daemon_end();

#endif
