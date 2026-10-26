# /*
#  * Makefile - CS50 Makefile for querier
#  *
#  * 
#  *
#  * Godwin Kangor, November 2024
#  */
# Makefile for the querier directory

# Makefile for the querier directory

OBJS = querier.o 
LIBS = ../common/common.a ../libcs50/libcs50-given.a 

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc
Make = make

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

querier:$(OBJS)
	$(MAKE) -C ../common
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

fuzzquery:$(OBJS_T)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


querier.o: querier.h 
fuzzquery.o: ../libcs50/file.h ../libcs50/mem.h

.PHONY: test valgrind clean


test: querier
	bash -v testing.sh >& testing.out
	

valgrind: 
	bash -v valgrind.sh &> valgrind.out
	
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f querier
	rm -f core
	$(MAKE) -C ../common clean

