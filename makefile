#
# Harlan James <root@aboyandhisgnu.org> <osurobotics.club>
#
# This file is part of Axolotl.
#
# Axolotl is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Axolotl is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Axolotl.  If not, see <http://www.gnu.org/licenses/>.
#

# Default Target
all: devel

# Compiling
CC = gcc

CFLAGS  = -std=gnu11 -Wall -pedantic -O3
CFLAGS += -Iinclude

LDFLAGS = -lfcgi -lcurl

# Default Search Locations
vpath %.c src
vpath %.h include
vpath %.a lib

# Files
SRC  = secio.c serialize.c termio.c
SRC += rcurl.c
SRC += rest_fpm.c

OBJ = $(SRC:.c=.o)
OBJDIR = obj
OBJPTH = $(addprefix $(OBJDIR)/, $(OBJ))

BIN = rest-fpm.fcgi
BINDIR = bin
BINPTH = $(addprefix $(BINDIR)/, $(BIN))

DIR = $(OBJDIR) $(BINDIR)

# Debugging
VARGS = --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --show-reachable=no --track-fds=yes

# Release Target
build: $(DIR) clean $(BINPTH)

# Development Target
devel: $(DIR) clean $(BINPTH)
	spawn-fcgi -n -p 8000 -- /usr/bin/valgrind $(VARGS) ./$(BINPTH)

# Compile Sources
$(BINPTH): $(OBJPTH)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $^ -o $@

# Create directories required after install
$(DIR):
	@for dir in $@; do \
		if [ ! -d "$$dir" ]; then \
			echo "Creating $$dir/"; \
			mkdir $$dir; \
		fi \
	done

clean:
	rm -f $(OBJPTH)
	rm -f $(BINPTH)
