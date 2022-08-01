# Makefile for common library that will be used by crawler, indexer, and querier
#	Builds and compiles all these utility functions into a library.
#
# Aimen Abdulaziz, Spring 2022

# object files, and the target library
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
LIBS = common.a 
OBJS = pagedir.o index.o word.o
Make = make

# make common.a
$(LIBS): $(OBJS) 
	ar cr $(LIBS) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h 
index.o: index.h
word.o: word.h

.PHONY: clean

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o