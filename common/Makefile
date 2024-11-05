# /*
#  * Makefile - CS50 Makefile for the common directory
#  *
#  * 
#  *
#  * Godwin Kangor, October 2024
#  */

# Compiler and flags
OBJS = pagedir.o index.o word.o
LIB = common.a
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50  # Ensure we include libcs50
MAKE = make

# Default target - build the library common.a
common.a: $(OBJS)
	ar rcs $(LIB) $(OBJS)

# Compilation rules for object files
pagedir.o: pagedir.c pagedir.h ../libcs50/webpage.h
	$(CC) $(CFLAGS) -c pagedir.c -o pagedir.o

index.o: index.c index.h ../libcs50/hashtable.h ../libcs50/counters.h
	$(CC) $(CFLAGS) -c index.c -o index.o

word.o: word.c word.h
	$(CC) $(CFLAGS) -c word.c -o word.o

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: clean

clean:
	rm -f *~ *.o *.a
	rm -f core
	rm -f common.a *~ *.o