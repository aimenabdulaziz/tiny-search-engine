# CS50 Lab 5
## TSE Indexer Design Spec
### Aimen Abdulaziz, Spring 2022

The job of the `indexer` is to read the documents in the `pageDirectory` output by the **crawler**, build an inverted index mapping from **words** to **documents**, and write that index to a file. 

### User interface

The indexer's only interface with the user is on the command-line; caller must always provide two arguments.

```console
./indexer pageDirectory indexFilename
```

For example:

```console
$ ./indexer /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-depth-2 data/letters-index-2
```
where `/thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-depth-2` contains a directory with the outputs produced by crawler.

`Indexer` does not assume the `pageDirectory` ends with slash at the end. It could be entered either with or without slash, and the program will handle it accordingly.

### Inputs and outputs

Input: the only inputs are command-line parameters; see the User Interface above. But the indexer reads from the `pageDirectory` provided by the user after concatenating 

Output: `indexer` saves the index file in the format specified in `REQUIREMENTS.md`. 

### Functional decomposition into modules

Anticipated modules or functions:

 1. *main* - parses arguments, initialize modules, and call other functions
 2. *validateArguments* - validates arguments 
 3. *indexBuild*, loops over all the documents produced by crawler to produced an in-memory index
 4. *indexPage* - reads every word in the webpage and insert to the index according to the specs

And some helper modules that provide data structures:

 1. *pagedir*, load webpages from crawler produced files 
 2. *index*, implements *hashtable* to provide in-memory index and functions to save and load index files.
 3. *webpage*, data structure to represent webpages
 4. *word*, normalize words

### Pseudo code for logic/algorithmic flow

The `indexer` will run as follows:

1. Process and validate command-line parameters
2. Initialize data structure index
3. indexBuild(directory, index)
4. indexSave(file, index);
5. clean up data structures

The `indextest` will run as follows:

1. Process and validate command-line parameters
2. Initialize data structure index
3. indexLoad(file1, index)
4. indexSave(file2, index)
5. clean up data structures

### Testing plan

*Integration testing*.  Assemble the `indexer` and test it as a whole. In each case, examine the outputs carefully to be sure they have the expected results. Then, run `indextest` to ensure the index can be loaded.

- test `indexer` with incorrect number of arguments
- test `indexer` with non-crawler produced directory 
- test `indexer` with non-existent path
- test `indexer` with valid arguments and compare my indexer outputs with the CS50 provided outputs
- run valgrind on both `indexer` and `indextest` to ensure there is no memory leak.