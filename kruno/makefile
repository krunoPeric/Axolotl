file: test.c
	gcc test.c -std=gnu99

setup_daemon: setup_daemon.c
	gcc setup_daemon.c -std=gnu99 -o setup_daemon.out

clean:
	rm -f *.txt *.out

cat:
	cat errfile.txt
	cat outfile.txt
	
