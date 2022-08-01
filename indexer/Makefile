# Makefile for 'indexer' module
#
# Aimen Abdulaziz, Spring 2022

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../common -I../libcs50
PROG1 = indexer
PROG2 = indextest
OBJS1 = indexer.o  
OBJS2 = indextest.o  
LLIBS = ../common/common.a ../libcs50/libcs50-given.a # local libraries
Make = make
TESTPROG = testing.sh
DATA = data

.PHONY: all test valgrind valgrindindexer valgrindindextest clean 

####### default ####### 
all: $(PROG1) $(PROG2)

####### indexer ####### 
$(PROG1): $(OBJS1) $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@ 

###### indextest ####### 
$(PROG2): $(OBJS2) $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@ 

####### test ####### 
test: $(TESTPROG) 
	chmod a+x $(TESTPROG)
	bash -v ./$(TESTPROG)
	# bash -v ./$(TESTPROG) &> testing.out # uncommment for automatic redirection to testing.out
	
####### valgrind ####### 	
valgrind: valgrindindexer valgrindindextest

# index valgrind
valgrindindexer: $(PROG1) 
	mkdir -p $(DATA)
	valgrind --leak-check=full --show-leak-kinds=all ./$(PROG1) /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-depth-5 data/letters-index-5

# indextest valgrind
valgrindindextest: $(PROG2)	
	mkdir -p $(DATA)
	valgrind --leak-check=full --show-leak-kinds=all ./$(PROG2) data/letters-index-5 data/letters-index-5-loaded
####### clean ####### 
clean:
	rm -f *~ *.o
	rm -rf *.dSYM # MacOS debugger info
	rm -f $(PROG1)
	rm -f $(PROG2)
	rm -rf $(DATA)