# /*
#  * Makefile - CS50 Makefile for indexer
#  *
#  * 
#  *
#  * Godwin Kangor, October 2024
#  */
# Makefile for the indexer directory

# Makefile for the indexer directory

OBJS = indexer.o 
OBJS_T = indextest.o
LIBS = ../common/common.a ../libcs50/libcs50-given.a 

# uncomment the following to turn on verbose memory logging
# TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc
Make = make

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: indexer indextest

indexer: $(OBJS)
	$(MAKE) -C ../common
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o indexer

indextest: $(OBJS_T)
	$(MAKE) -C ../common
	$(CC) $(CFLAGS) $(OBJS_T) $(LIBS) -o indextest

indexer.o: indexer.c ../common/pagedir.h ../common/index.h ../common/word.h
	$(CC) $(CFLAGS) -c indexer.c -o indexer.o

indextest.o: indextest.c ../common/index.h
	$(CC) $(CFLAGS) -c indextest.c -o indextest.o

.PHONY: test valgrind clean

test: all
	bash -v testing.sh > test_output.txt

valgrind: 
	bash -v valgrind.sh &> valgrind.out

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f indexer
	rm -f indextest
	rm -f core
	$(MAKE) -C ../common clean