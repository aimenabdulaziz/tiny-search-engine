# CS50 Lab 5
## Aimen Abdulaziz, Spring 2022

### Indexer

`indexer` reads the documents in the `pageDirectory` which contains outputs produced by **crawler** to build an inverted index mapping from **words** to **documents**, and write that index to a file. Read the **Markdown** files for more details.

### Usage

You should provide arguments for `indexer` in the following format: 
```console 
./indexer pageDirectory indexFilename
```

You should provide arguments for `indextest` in the following format: 
```console 
./indextest oldIndexFilename newIndexFilename
```

### Exit Status

Indexer:
- 0: successful run
- 1: incorrect number of arguments
- 2: argument 1 (pageDirectory) is not the pathname of a directory produced by the Crawler
- 3: argument 2 (indexFilename) cannot be opened for writing
- 99: null pointer while allocating memory or opening a file

Indextest:
- 0: successful run
- 11: incorrect number of arguments
- 12: argument 1 (oldIndexFilename) is not readable
- 13: argument 2 (newIndexFilename) is not writable

### Assumptions

The `pageDirectory` provided by the user does not contain any files whose name is an integer (i.e., 1, 2, ...)

The content of files in `pageDirectory` follow the format defined in the specs (i.e first line is URL, second line is depth, and the remaining of the file is html)

For `indexer`:
- `pageDirectory` is the pathname for an existing directory produced by crawler
- `indexFilename` is the pathname of a file into which the index should be written

For `indextest`:
- `oldIndexFilename` is the name of a file produced by the indexer
- `newIndexFilename` is the name of a file into which the index should be written

### Compilation

To compile both indexer and indextest, simply run `make`. 

To compile only indexer, run `make indexer`

To compile only indextest, run `make indextest`

To run automated test cases, simply execute `make test`. The testing script will take care of compiling the required executables. Feel free to redirect the output to your desired path. You can run `make test &> path`, where `path` is your desired location. 

I didn't include test cases that would take long time in the testing script, but feel free to test the program with your preferred test cases.

To test both indexer and indextest with valgrind, run `make valgrind`. But to run one at a time, execute `valgrind indexer` or `valgrind indextest`. I have added a test case in the `Makefile`, but feel free to modify that and test with your preferred test case.

To clean all the files created during compilation and testing, run `make clean`.
