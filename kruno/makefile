CC = gcc

CFLAGS = -std=gnu99 -Wall -pedantic 
#CFLAGS += -DDAEMON
CFLAGS += -DCURLING -lcurl

SRCS = main.c src/daemon.c src/curl_functions.c
HEADERS = headers/daemon.h headers/curl_functions.h

PROG = bin/warden_main

$(PROG) : $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) $(HEADERS) -o $(PROG)

clean :
	rm -f *.log *.out *.o $(PROG)
