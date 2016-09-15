CC = gcc

CFLAGS = -std=gnu99 -Wall -pedantic 
CFLAGS += -DDAEMON
CFLAGS += -DCURLING 
CFLAGS += -lcurl
CFLAGS += -DJSMN
CFLAGS += -L../jsmn -ljsmn

SRCS = main.c src/daemon.c src/curl_functions.c src/jsmn_functions.c
SRCS += ../jsmn/jsmn.c
HEADERS = include/daemon.h include/curl_functions.h include/jsmn_functions.h
HEADERS += ../jsmn/jsmn.h

PROG = bin/warden_main

$(PROG) : $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) -o $(PROG)

clean :
	rm -f *.log *.out *.o $(PROG)
