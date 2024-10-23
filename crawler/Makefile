
# /*
#  * Makefile - CS50 Makefile
#  *
#  * 
#  *
#  * Godwin Kangor, October 2024
#  */
 # Makefile for the crawler directory
OBJS = crawler.o 
L = ../libcs50
C = ../common
LLIBS = $C/common.a $L/libcs50.a  # Correctly link to the built libcs50.a
CC = gcc
CFLAGS = -Wall -pedantic -std=gnu11 -ggdb -I$L -I$C
MAKE = make

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: all clean valgrind test

all: crawler

crawler: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) -o $@

crawler.o: crawler.c $L/webpage.h $C/pagedir.h $L/bag.h $L/hashtable.h $L/mem.h
	$(CC) $(CFLAGS) -c crawler.c -o crawler.o

############## Run tests ##########
test: testing.sh 
	bash -v testing.sh

############## Run Valgrind ##########
valgrind: crawler
	$(VALGRIND) ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 2

############## Clean ##########
clean:
	rm -f *~ *.o crawler
	rm -f core

