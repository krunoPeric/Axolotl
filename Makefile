# Makefile for building restCGI for Warden

all: devel

CC          = gcc
BIN         = rest-fpm.fcgi
SRC         = src/rest_fpm.c
OBJ         = $(SRC:.c=.o)
CFLAGS      = -std=gnu99 -c -C -g -Wall -pedantic
CFLAGS     += -Iinclude
LDFLAGS     = -lfcgi -lcurl

build: clean $(BIN)
devel: clean $(BIN)
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --trace-children=yes spawn-fcgi -p 8000 -n ./$(BIN)
$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $(*D)/$(*F).c -o $@

clean:
	rm -f -v $(OBJ) $(BIN)
