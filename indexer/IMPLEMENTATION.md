# CS50 Lab 5
## TSE Indexer Implementation Spec
### Aimen Abdulaziz, Spring 2022

### Indexer

The job of the `indexer` is to read the documents in the `pageDirectory` output by the **crawler**, build an inverted index mapping from **words** to **documents**, and write that index to a file. It’s called an inverted index because it maps from words to documents, the opposite of a document (itself a sequence of words). Indexer starts mapping the documents after crawler is complete. Later, the querier will read the index and use it to answer queries. 

### Incorporated Modules

- `word` - contains a function that normalize a word by converting each character to lowercase
- `index` - implements a `hashtable` of `countersets` to map from a word to a list of (docID, count) pairs
- `memory` - if pointer p is NULL, print error message to stderr and die.
- `pageDir` - common page directory module used by crawler, indexer, and querier.
- `webpage` - utility functions for downloading, saving, and loading web pages. 

### Implementation

The *indexer* module implemented in `indexer.c` leverage the `word`, `index`, `memory`, `pageDir`, and `webpage` to read the documents in the `pageDirectory` outputs and index the documents. It includes the following functions:

```c
/* Parse arguments and call other methods */
int main(const int argc, char *argv[]);

/* Helper function to parse and validates the arguments 
 * 
 * Validate:
 *    arg1 is a crawler produced directory 
 *    arg2 is an existing directory with writing permission
 */
static void validateArguments(char *URL, char *pageDirectory, int maxDepth);

/* Loops over all the documents produced by crawler to produced an in-memory index 
 * 
 * Assumptions: 
 *   PageDirectory has files named 1, 2, 3, ..., without gaps.
 *   The content of files in pageDirectory follow the format defined 
 *      in the specs (i.e first line is URL, second line is depth, and the remaining of the file is html)
 *
 *  Until all the documents produced by crawler are exhausted
 *      index each page
 *      free the webpage
 *      increment the docID to read the next document
 * 
 */
static index_t *indexBuild(char *pageDirectory);

/* Read every word in the webpage and insert to the index according to the specs
 * 
 * Check whether the word exists in the index
 *  If successful, 
 *      check if the docID exists in the ctrs of the index
 *          If successful, 
 *              increment the count
 *          else,
 *              add the docID to the ctrs
 *  Else,
 *      create a new ctrs and add the docID
 * 
 * IndexPage ignores words with less than 3 characters
 */
static void indexPage(webpage_t *page, index_t *index, int docID);
```

### Files

- `Makefile` - compilation procedure
- `indexer.c` - the implementation
- `DESIGN.md` - specifies the structure of indexer in a language-independent, machine-dependent way
- `IMPLEMENTATION.md` - specifies the language-dependent, machine-dependent details of the implementation
- `REQUIREMENTS.md` -  specifies what indexer must do
- `testing.sh` - automates tests by running a bunch of test cases
- `testing.out` - outputs of test results from `testing.sh` 
- `indextest.c` - loads the `index` from the `oldIndexFilename` (output of *indexer*) and save the file to `oldIndexFilename`
