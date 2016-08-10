# Makefile for building restCGI for Warden

all: devel

CC          = gcc
BIN         = rest-fpm.fcgi
SRC         = src/termio.c src/secio.c src/serialize.c src/rcurl.c src/rest_fpm.c
OBJ         = $(SRC:.c=.o)
CFLAGS      = -std=gnu11 -c -Wall -pedantic
CFLAGS     += -Iinclude
DFLAGS      = -std=gnu11 -c -C -g -D DEBUG -Wall -pedantic
DFLAGS     += -Iinclude
LDFLAGS     = -lfcgi -lcurl

build: clean $(BIN)

devel: clean $(BIN)
	#valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --trace-children=yes spawn-fcgi -p 8000 -n ./$(BIN)
	spawn-fcgi -n -p 8000 -- /usr/bin/valgrind --tool=memcheck --leak-check=full --trace-children=yes ./$(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@

$(OBJ): $(SRC)
	$(CC) $(DFLAGS) $(*D)/$(*F).c -o $@

clean:
	rm -f -v $(OBJ) $(BIN)
