# CS50 Lab 6
## TSE Querier Design Spec
### Aimen Abdulaziz, Spring 2022

**TSE Querier** is a standalone program that reads the index file produced by the **TSE Indexer**, and page files produced by the **TSE Crawler** to answers search queries submitted via **stdin**.

### User interface

The indexer's only interface with the user is on the command-line; caller must always provide two arguments.

```console
./querier pageDirectory indexFilename
```

For example:

```console
dir=/thayerfs/courses/22spring/cosc050/cs50tse/tse-output
$ ./querier $dir/letters-depth-5 $dir/letters-index-5
```
where `$dir` contains a directory with the outputs produced by crawler and indexer.

`Querier` does not assume the `pageDirectory` and `indexFilename` ends with slash at the end. It could be entered either with or without slash, and the program will handle it accordingly.

### Inputs and outputs

Input: the only inputs are command-line parameters; see the User Interface above. 

Output: `querier` prints the query result as per the format specified in `REQUIREMENTS.md`. 

### Functional decomposition into modules

Anticipated modules or functions:

 1. *main* - parses arguments, initialize modules, and call other functions
 2. *validateArguments* - validates arguments 
 3. *handleInput* - reads search queries from stdin, one per line, until EOF and invoke processQuery for each
 4. *processQuery* - loops over all the words in the array and build an accumulator ctrs, which is passed to printResult
 5. *printResult* - ranks the resulting set of documents according to its score, and print the set of documents in decreasing rank order

And some helper modules that provide data structures:

 1. *pagedir*, load webpages from crawler produced files 
 2. *index*, implements *hashtable* to provide in-memory index and functions to save and load index files.
 3. *webpage*, data structure to represent webpages
 4. *word*, normalize words

### Pseudo code for logic/algorithmic flow

The `querier` will run as follows:

1. Process and validate command-line parameters
2. Load index from `indexFilename` and save it in `index_t` data structure
3. Call handleInput(index, pageDirectory), which parses user query as per the specs and call processQuery accordingly
4. clean up data structures


### Testing plan

*Integration testing*.  Assemble the `querier` and test it as a whole. In each case, examine the outputs carefully to be sure they have the expected results.

- test `querier` with incorrect number of arguments
- test `querier` with non-crawler produced directory 
- test `querier` with unreadable file
- test `querier` with valid arguments and invalid queries that have syntax errors
- test `querier` with valid arguments and valid queries
- run valgrind to ensure there is no memory leak.