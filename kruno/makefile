file: test.c
	gcc test.c -std=gnu99

setup_daemon: setup_daemon.c
	gcc setup_daemon.c -std=gnu99 -o setup_daemon.out

clean:
	rm -f *.txt *.out *.log
	
kruno_daemon: daemon.c
	gcc daemon.c -std=gnu99 -o kruno_daemon.out
