#!/bin/bash
# testing.sh - perform automatic tests for indexer to confirm it works as intended
#
# usage: run 'make test' and redirect to a directory as needed 
#
# Aimen Abdulaziz, Spring 2022

# make indexer and indextest
make

# directory for crawler and indexer outputs provided by CS50
dir=/thayerfs/courses/22spring/cosc050/cs50tse/tse-output/

# make data directory in indexer 
rm -rf data # delete if the directory exists
mkdir -p data

################ invalid arguments ################
# incorrect number of arguments 
./indexer $dir/letters-depth-1 data/test hello

# non-crawler produced directory (invalid arg1)
./indexer hello data/test 

# unwriteable index file -> indexSave can't write to a directory
mkdir directory # make a directory
./indexer $dir/letters-depth-1 directory
rm -rf directory # remove directory after testing

################ indexer ################
# indexer using crawler produced letter-depth-2
./indexer $dir/letters-depth-2 data/letters-index-2

# compare my indexer output with the CS50 provided output
sort data/letters-index-2 > data/letters-index-2-sorted
sort $dir/letters-index-2 > data/letters-index-2-sorted-compare
diff data/letters-index-2-sorted data/letters-index-2-sorted-compare 

# indexer using crawler produced wikipedia-depth-1
./indexer $dir/wikipedia-depth-1 data/wikipedia-index-1

# compare my indexer output with the CS50 provided output
sort data/wikipedia-index-1 > data/wikipedia-index-1-sorted
sort $dir/wikipedia-index-1 > data/wikipedia-index-1-sorted-compare
diff data/wikipedia-index-1-sorted data/wikipedia-index-1-sorted-compare 

# indexer using crawler produced toscrape-depth-2
./indexer $dir/toscrape-depth-2/ data/toscrape-index-2

# compare my indexer output with the CS50 provided output
sort data/toscrape-index-2 > data/toscrape-index-2-sorted
sort $dir/toscrape-index-2 > data/toscrape-index-2-sorted-compare
diff data/toscrape-index-2-sorted data/toscrape-index-2-sorted-compare

################ indexer valgrind ################
# check memory leak for indexer
valgrind --leak-check=full --show-leak-kinds=all ./indexer $dir/letters-depth-5 data/letters-index-5

################ indextest ################
# test 1: load index and re-create
./indextest data/toscrape-index-2 data/toscrape-index-2-loaded

# sort and compare original index with the re-created index
sort data/toscrape-index-2-loaded > data/toscrape-index-2-loaded-sorted
diff data/toscrape-index-2-sorted data/toscrape-index-2-loaded-sorted 

# test 2: load index and re-create
./indextest data/letters-index-5 data/letters-index-5-loaded

# sort and compare original index with the re-created index
sort data/letters-index-5 > data/letters-index-5-sorted
sort data/letters-index-5-loaded > data/letters-index-5-loaded-sorted
diff data/letters-index-5-sorted data/letters-index-5-loaded-sorted 

################ indextest valgrind ################
# check memory leak for indextest
valgrind --leak-check=full --show-leak-kinds=all ./indextest data/wikipedia-index-1 data/wikipedia-index-1-loaded