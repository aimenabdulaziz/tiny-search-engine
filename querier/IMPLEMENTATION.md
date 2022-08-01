# CS50 Lab 6
## TSE Querier Implementation Spec
### Aimen Abdulaziz, Spring 2022

### Querier

`Querier` reads the index produced by the Indexer and the page files produced by the Crawler, to interactively answer written queries entered by the user. Our `Querier` loads the index into memory (a data structure we developed for the Indexer) and then prompts the user for queries. 

### Incorporated Modules

- `word` - contains a function that normalize a word by converting each character to lowercase
- `index` - implements a `hashtable` of `countersets` to map from a word to a list of (docID, count) pairs
- `memory` - if pointer p is NULL, print error message to stderr and die.
- `pageDir` - common page directory module used by crawler, indexer, and querier.
- `webpage` - utility functions for downloading, saving, and loading web pages. 
- `file` - read a word, line, or entire file either from stdin or fp
- `counters` - a counter set is a set of counters, each distinguished by an integer key

### Implementation

The *querier* module implemented in `querier.c` leverage the `word`, `index`, `memory`, `pageDir`, `webpage`, `file`, and `counters` to read the documents in the `pageDirectory` outputs and `indexFilename` to answer queries invoked by user. It includes the following functions:

**main**
```c
/* Parse arguments and call other methods */
int main(const int argc, char *argv[])
```

**validateArguments**
```c
/* Helper function to validate the arguments 
 * 
 * Validate:
 *    arg1 is the pathname of a directory produced by the Crawler
 *    arg2 is the pathname of a file produced by the Indexer (readable file)
 * 
 * Function exits with an error code if there is any problem in the verification process
 */
static void validateArguments(const int argc, char *argv[], char **pageDirectory, char **indexFilename)
```

**handleInput**
```c
/* Read search queries from stdin, one per line, until EOF and invoke processQuery for each
 * Prompt user for input
 * Delimit search queries by white space, new line, and tab
 * Ensure each character is each word is an alphabet and the query doesn't violate the specs
 *     if unsuccessful (there is syntax error), 
 *          print an error message and exit
 *      else,
 *          normalize (convert to lowercase) each character and add to words array
 *          call processQuery
 */
static void handleInput(index_t *index, char *pageDirectory)
```

**hasSyntaxError**
```c
/* Helper function to perform extensive tests based on the specs to verify if the input has syntax error
 * Return boolean:
 *      true, if:
 *          'and' / 'or' appear at the beginning or end of a query
 *          'and' & 'or' appear consecutively 
 *      otherwise, false
 */
static bool hasSyntaxError(char *words[], int numWords, bool first, bool last)
```

**compareStrings**
```c
/* Helper function for validating querier by comparing two consecutive words in an array with two strings
 * Return:
 *      true, if the str1 is equal to the first word and str2 is equal to the second word
 *      otherwise, false
 */
static bool compareStrings(char *words[], char *str1, char *str2, int i)
```

**prompt**
```c
/* print a prompt for an interactive user */
static void prompt(void)
```

**printQuery**
```c
/* Loop over all the words in the array and print a clean query
 * This is the same query that will be processed by the Tiny Search Engine
 */
static void printQuery(char *words[], int numWords)
```

**processQuery**
```c
/* Loop over all the words in the array and build an accumulator ctrs
 * While looping, 'and' has higher precedence than 'or'
 * Create result ctrs and initalze it 
 * Create temo ctrs and set it to NULL
 * Start looping over all the words in the array
 *      if (reset),
 *          initialize temp ctrs to new ctrs
 *      if word == 'or',
 *          merge temp ctrs with result
 *          delet temp ctrs
 *          reset
 *      if word == 'and',
 *          pass
 *      if word != "and" && word != or,
 *          find ctrs for word in the index (indexFind)
 *          if temp ctrs is empty,
 *              copy everything in ctrs returned from indexFind
 *          else,
 *              find the intersection of temp ctrs and the ctrs returned from indexFind
 * 
 * Merge result with temp ctrs
 * Free memory
 */
static void processQuery(index_t *index, char *words[], int numWords, char *pageDirectory)
```

**countersIntersect**
```c
/* Invoked when handling andSequence
 * For two counters,
 *      initialize a struct that holds two ctrs where the first ctrs will be treated as the result ctrs
 *      iterate through ctr1 and pass the struct hold two ctrs and intersectHelper
 */
void countersIntersect(counters_t* ct1, counters_t* ct2)
```

**intersectHelper**
```c
/* Helper function for coutersIntersect
 * If the ctrs have the same key, 
 *      store the min count for the specific key as count in result ctrs
 * Else,
 *      store 0 as the count in the result ctrs 
 */
static void intersectHelper(void *arg, const int key, const int count)
```

**min**
```c
/* Given two integers, it will return the smallest one */
static inline int min(const int a, const int b) 
```

**countersMerge**
```c
/* Merge two ctrs
 * If either of the two ctrs is NULL,
 *      don't do anything
 * Else,
 *      iterate through each and store the result in the first ctrs
 */
void countersMerge(counters_t* ct1, counters_t* ct2)
```

**mergeHelperFirst**
```c
/* Iterate through the keys of ct1 and merge with ct2 */
static void mergeHelperFirst(void *arg, const int key, const int count)
```

**mergeHelperSecond**
```c
/* Iterate through the keys of ct2 and merge with ct1 */
static void mergeHelperSecond(void *arg, const int key, const int count)
```

**printResult**
```c
/* Rank the resulting set of documents according to its score, 
 * and print the set of documents in decreasing rank order
 * If count for non-zero count in ctrs is 0, 
 *      print "No documents match."
 * Else,
 *      make an Array of array (keyCountPairs) where each index is key,count pair
 *      pass the array to qsort and rank in decreasing order of count
 *      for each ctrs in results ctrs,
 *          obtain the URL by reading the first line of the relevant document file
 *          obtain key and count from the array we created using makeArray
 *      free keyCountPairs array
 */
static void printResult(counters_t *ctrsAccumulator, char *pageDirectory)
```

**ctrsItemCount**
```c
/* count the non-null items in the counters */
static void ctrsItemCount(void *arg, const int key, const int count)
```

**nonZeroItemCount**
```c
/* total number of key,count pairs with non-zero count value */
static void nonZeroItemCount(void *arg, const int key, const int count)
```

**makeArray**
```c
/* Create and return a nested array where each index in the array is a key,count pair
 * This array is later passed to qsort for ranking in decreasing 
 * User is responsible for later calling freeKeyCountPairs
 */
static int **makeArray(counters_t *ctrs, int numElements)
```

**makeArrayHelper**
```c
/* Helper function for makeArray
 * In each index,
 *      add key at idx 0 and count at idx 1
 * Increment the current index while adding in the nested array
 */
static void makeArrayHelper(void *arg, const int key, const int count)
```

**comparator**
```c
/* Comparator passed to qsort -> follows the format qsort takes 
 * Designed to sort in decreasing order of scores
 */
static int comparator(const void* a, const void* b)
```

**getURL**
```c
/* Obtain the URL by reading the first line of the relevant document file from the pageDirectory */
static void getURL(char *pageDirectory, int fileID, char **URL)
```

### Query Syntax

- the literal 'and' must come between words, and the literal 'or' must come between andsequences, so they cannot appear at the beginning or end of a query,
- the literals ('and' & 'or') cannot be adjacent
- characters other than letters or spaces are disallowed.

### Files

- `Makefile` - compilation procedure
- `querier.c` - the implementation
- `DESIGN.md` - specifies the structure of indexer in a language-independent, machine-dependent way
- `IMPLEMENTATION.md` - specifies the language-dependent, machine-dependent details of the implementation
- `REQUIREMENTS.md` -  specifies what indexer must do
- `testing.sh` - automates tests by running a bunch of test cases
- `testing.out` - outputs of test results from `testing.sh` 
- `queries.test` - test cases used while running `testing.sh`
- `fuzzquery.c` - generates a series of random queries for testing querier