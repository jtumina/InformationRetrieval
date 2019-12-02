.PHONY: clean build all

# compilation settings
CC = gcc
CFLAGS = -I$(INCLUDE_DIR) -Wall -Wextra -Werror -pedantic -std=gnu99 -pedantic -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition -g

# directory paths
INCLUDE_DIR = ./include/
SRC_DIR = ./src/
OBJ_DIR = ./obj/

# file lists
CFILES = hashtable.c infoRetrieval.c sort.c search.c
OBJS = hashtable.o infoRetrieval.o sort.o search.o

# binary
BIN = search

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BIN): $(OBJS)
	$(CC) -o $@ $(OBJS)

clean:
	rm -f $(OBJS) $(BIN) *~
