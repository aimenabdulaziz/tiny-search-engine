# CS50 Lab 4
## TSE Crawler Implementation Spec
### Aimen Abdulaziz, Spring 2022

### Crawler

A `crawler` crawls the web from a given seed to a given maxDepth and caches the content of the pages it finds, one page per file, in a given directory. It only crawls internal URLs within the CS50 playground. Internal URLs must begin with `http://cs50tse.cs.dartmouth.edu/`.

### Incorporated Modules

- `bag` - an unordered collection of `*void` _items_. `Crawler` use `bag` to store page that are yet to explore.
- `hashtable` - a set of (key,item) pairs. `Crawler` use `hashtable` to store URLs we've seen so far.
- `memory` - if pointer p is NULL, print error message to stderr and die.
- `pageDir` - common page directory module used by crawler, indexer, and querier.
- `webpage` - utility functions for downloading, saving, and loading web pages. 

### Implementation

The *cralwer* module implemented in `crawler.c` leverage the `bag`, `hashtable`, `memory`, `pageDir`, and `webpage` to crawl *websites*. It includes the following functions:

```c
/* parse arguments and call other methods */
int main(const int argc, char *argv[]);

/* helper function to parse and validates all three arguments */
int validateArguments(char *URL, char *pageDirectory, int maxDepth);

/*
 * initalize bag and hashtable modules
 * bag stores webpages we have yet to explore and hashtable stores URLs we have seen so far
 * loop over pages to explore until all the webpages in the bag are exhausted 
 * try to fetch each webpage webpage and call pageSaver, pageScanner, and webpage_delete appropriately 
 * free the bag and hashtable once everything is done
 */
int crawler(char *URL, char *pageDirectory, int maxDepth);

/* 
 * extract URLs from a page and process each one until current depth is equal max depth
 * print logs along the away
 * if URL is internal, make a new webpage. Otherwise, do nothing
 * insert the webpage to the bag, and the URL is the hashtable
 * free extracted URL from the hashtable if the URL is external or duplicate
 */
void pageScanner(webpage_t *page, bag_t *toVisit, hashtable_t *seen, int maxDepth);

/*
 * log one word (1-9 chars) about a given url and print the log
 */
inline static void logr(const char *word, const char *url, const int depth);
```

### Files

- `Makefile` - compilation procedure
- `crawler.c` - the implementation
- `DESIGN.md` - specifies the structure of crawler in a language-independent, machine-dependent way
- `IMPLEMENTATION.md` - specifies the language-dependent, machine-dependent details of the implementation
- `REQUIREMENTS.md` -  specifies what crawler must do
- `testing.sh` - automate tests by running a bunch of test cases
- `testing.out` - output of test results from `testing.sh` 
