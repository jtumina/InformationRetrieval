.PHONY: clean build all

# compilation settings
CC = gcc
CFLAGS = -I$(INCLUDE_DIR) -Wall -Wextra -Werror -pedantic -std=gnu99 -pedantic -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition -g

# directory paths
INCLUDE_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj

# file lists
CFILES = $(wildcard $(SRC_DIR)/*.c)
OBJS = ./obj/hashtable.o ./obj/infoRetrieval.o ./obj/sort.o ./obj/main.o

# binary
BIN = main

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BIN): $(OBJS)
	$(CC) -o $@ $(OBJS) -lm

clean:
	rm -f $(OBJS) $(BIN) search_scores.txt *~
