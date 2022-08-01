# CS50 Lab 6
## Aimen Abdulaziz, Spring 2022

### Querier

The third component of the Tiny Search Engine, `Querier`, reads the index produced by the Indexer and the page files produced by the Crawler, to interactively answer written queries entered by the user. The querier supports ‘and’ and ‘or’ operators where 'and' has higher precedence. Then it prints the document set in decreasing order of score (count). The `querier` meets the fulls specs specified in `REQUIREMENTS.md`. Read the Markdown files for more details on the specs and implementations.  

### Usage

You should provide arguments for `querier` in the following format: 
```console 
./querier pageDirectory indexFilename
```

### Exit Status
- 0: successful run
- 1: incorrect number of arguments
- 2: argument 1 (pageDirectory) is not the pathname of a directory produced by the Crawler
- 3: argument 2 (indexFilename) is not the pathname of a readable file

### Assumptions

The crawler produced `pageDirectory` provided by the user has files named 1, 2, 3, ..., without gaps.

The content of files in `pageDirectory` follows the format defined in the specs

The content of the file named by `indexFilename` follows the Index file format

Querier arguments:
- `pageDirectory` is the pathname for an existing directory produced by crawler
- `indexFilename` is the name of a file produced by the indexer

### Compilation

To compile `querier`, run `make`. 

To compile `fuzzquery`, run `make fuzzquery`

To run automated test cases, simply execute `make test`. The testing script will take care of compiling the required executables. Feel free to redirect the output to your desired path. For redirecting the output, run `make test &> path`, where `path` is your desired location. 

I didn't include test cases that would take long time in the testing script, but feel free to test the program with your preferred test cases.

To test `querier` valgrind, run `make valgrind`. I have added a test case in the `Makefile`, but feel free to modify that and test with your preferred test case.

To clean up all the files created during compilation and testing, run `make clean`.

