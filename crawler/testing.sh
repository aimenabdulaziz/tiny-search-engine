#!/bin/bash
# testing.sh - perform tests for crawler to confirm it works as intended
#
# usage: run 'make test' 
#
# the output of the file will be redirected to testing.out
#
# Aimen Abdulaziz, Spring 2022

# make crawler
make

# make directory for writing the crawler output
rm -rf data # delete if the directory exists
mkdir -p data/test
for i in {1..5}; do
    mkdir -p "data/test$i"
done

# incorrect number of arguments 
echo "test 1"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/test 6 1

# non-internal seedURL
echo "test 2"
./crawler https://www.google.com/ data/test 1

# non-existent directory
echo "test 3"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/non 6

# invalid integer for depth
echo "test 4"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/test 1a

# invalid depth (out of the range between 0-10)
echo "test 5"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/test -1

echo "test 6"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/test 12

# non-existent server seedURL
echo "test 7"
./crawler http://aimen.com data/test 6

# valid server but non-existent page 
echo "test 8"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/aimen.html data/test 7

# crawl a valid internal seedURL that has duplicate and external URLs
echo "test 9"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/test 2

# crawl to depth 5 
echo "test 10"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/test1 5

# crawl from a page at depth 1 of http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
echo "test 11"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/A.html data/test2 5

# crawl the cs50 Wikipedia playground at depth 1
echo "test 12"
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html data/test3 1

# crawl the cs50 toscrape playground at depth 0
echo "test 13"
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html data/test4 0

# valgrind to check memory leak
echo "valgrind test"
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/test5 2

# crawl the cs50 Wikipedia playground at depth 2
# WARNING: this test takes approximately 45 minutes 
# uncomment if you want to run
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html data/test6 2 