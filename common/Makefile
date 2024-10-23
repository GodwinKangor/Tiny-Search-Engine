
# /*
#  * Makefile - CS50 Makefile
#  *
#  * 
#  *
#  * Godwin Kangor, October 2024
#  */
 # Makefile for the common directory

# Compiler and flags
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50  # Ensure we include libcs50

# Default target - build the library common.a
common.a: pagedir.o
	ar rcs common.a pagedir.o

# Object files
pagedir.o: pagedir.c pagedir.h
	$(CC) $(CFLAGS) -c pagedir.c

# Clean up object files and library
clean:
	rm -f *.o common.a

