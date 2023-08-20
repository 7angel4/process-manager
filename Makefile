# Makefile for Project 1
# Created by Angel He (angelh1@student.unimelb.edu.au) 24/03/2023

# define C compiler and flags
CC = gcc
CFLAGS = -Wall -g
LIB = -lm

# define sets of header source files and object files
SRC = main.c process_data.c queue.c linked_list.c management.c memory.c doubly_linked_list.c communicator.c
OBJ = $(SRC:.c=.o)

# define the executables
EXE = allocate

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) $(LIB)

%.o: %.c %.h

main.o: management.h

process_data.o: doubly_linked_list.h

queue.o: process_data.h linked_list.h

management.o: queue.h memory.h communicator.h

memory.o: process_data.h doubly_linked_list.h

communicator.o: process_data.h

clean:
	rm -f $(OBJ) $(EXE)

format:
	clang-format -style=file -i *.c *.h