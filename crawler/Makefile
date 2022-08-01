# Makefile for 'crawler' module
#
# Aimen Abdulaziz, Spring 2022

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../common -I../libcs50
PROG = crawler
OBJS = crawler.c 
LLIBS = ../common/common.a ../libcs50/libcs50-given.a # local libraries
Make = make
TESTPROG = testing.sh
DATA = data

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@ 

.PHONY: test valgrind clean 

test: $(TESTPROG) 
	chmod a+x $(TESTPROG)
	bash -v ./$(TESTPROG)
	# bash -v ./$(TESTPROG) &> testing.out # uncommment for automatic redirection to testing.out
	
valgrind: $(PROG) 
	mkdir -p data/test6
	valgrind --leak-check=full --show-leak-kinds=all ./$(PROG) http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/test6 3

clean:
	rm -f *~ *.o
	rm -rf *.dSYM # MacOS debugger info
	rm -f $(PROG)
	rm -rf $(DATA)