#!/bin/bash
# testing.sh - perform automatic tests for querier to confirm it works as intended
#
# usage: run 'make test' and redirect to a directory as needed 
#
# Aimen Abdulaziz, Spring 2022

# make querier and fuzzquery
make
make fuzzquery

# directory for crawler and indexer outputs provided by CS50
dir=/thayerfs/courses/22spring/cosc050/cs50tse/tse-output

################ invalid arguments ################
# incorrect number of arguments 
./querier $dir/letters-depth-5 $dir/letters-index-5 hello

# non-crawler produced directory (invalid arg1)
./querier $dir/hello $dir/letters-index-5

# file is not readable (invalid arg2)
./querier $dir/letters-depth-5 $dir/hello

################ query syntax ################
./querier $dir/letters-depth-5 $dir/letters-index-5 < queries.test

################ randomly generated queries by fuzzQuery ################
./fuzzquery $dir/letters-index-3 10 2 | ./querier $dir/letters-depth-3 $dir/letters-index-3
./fuzzquery $dir/wikipedia-index-1 10 2 | ./querier $dir/wikipedia-depth-1 $dir/wikipedia-index-1

################ valgrind ################
valgrind --leak-check=full --show-leak-kinds=all ./querier $dir/toscrape-depth-2 $dir/toscrape-index-2 < queries.test