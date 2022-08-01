# Makefile for querier
#
# Aimen Abdulaziz, Spring 2022
# uncomment to view debugging logs
# TESTING = -DDEBUG

D = /thayerfs/courses/22spring/cosc050/cs50tse/tse-output

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../common -I../libcs50
PROG = querier
OBJS = querier.c  
PROG1 = fuzzquery
OBJS1 = fuzzquery.c 
LLIBS = ../common/common.a ../libcs50/libcs50-given.a # local libraries
Make = make
TESTPROG = testing.sh
DATA = data

####### querier ####### 
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@ 

.PHONY: fuzzquery test valgrind clean 

####### fuzzquery #######
$(PROG1): $(OBJS1) $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@ 
	
####### test ####### 
test: $(TESTPROG) 
	chmod a+x $(TESTPROG)
	bash -v ./$(TESTPROG)
	# bash -v ./$(TESTPROG) &> testing.out # uncommment for automatic redirection to testing.out
	
####### valgrind ####### 	
valgrind: $(PROG) 
	valgrind --leak-check=full --show-leak-kinds=all ./$(PROG) $D/letters-depth-6  $D/letters-index-6 < queries.test

####### clean ####### 
clean:
	rm -f *~ *.o
	rm -rf *.dSYM # MacOS debugger info
	rm -f $(PROG)
	rm -f $(PROG1)
	rm -rf $(DATA)