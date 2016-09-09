CC = gcc

CFLAGS = -std=gnu99 -Wall -pedantic 
#CFLAGS += -DDAEMON
CFLAGS += -DCURLING -lcurl

SRCS = main.c src/daemon.c
HEADERS = headers/daemon.h

PROG = bin/warden_main

$(PROG) : $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) $(HEADERS) -o $(PROG)
