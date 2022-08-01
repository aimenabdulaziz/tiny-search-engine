/* 
 * indexer.c -  reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. 
 *
 * usage: ./indexer pageDirectory indexFilename
 * 
 * see .md files for further details
 * 
 * Aimen Abdulaziz, Spring 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagedir.h"
#include "index.h"
#include "memory.h"
#include "webpage.h"
#include "word.h"
#include "counters.h"

/********************** local function prototypes ********************* */
static void validateArguments(char *pageDirectory, char *indexFilename);
static index_t *indexBuild(char *pageDirectory);
static void indexPage(webpage_t *page, index_t *index, int docID);

/* ******************** main ******************** */
/* parse arguments and call other methods */
int main(const int argc, char *argv[])
{
    char *program = argv[0];
    // validate number of arguments
    if (argc != 3){
        fprintf(stderr, "usage: %s pageDirectory indexFilename\n", program);
        exit(1);
    }

    // parse arguments
    char *pageDirectory = argv[1]; // pathname of crawler produced directory
    char *indexFilename = argv[2]; // writeable file for writing index

    // validate arguments
    validateArguments(pageDirectory, indexFilename);

    // build index 
    index_t *index = indexBuild(pageDirectory); 

    // save index to an external file
    indexSave(index, indexFilename);

    // delete index in memory
    indexDelete(index);

    return 0;
}

/**************** validateArguments() ****************/
/* Helper function to validate the arguments 
 * 
 * Validate:
 *    arg1 is a crawler produced directory 
 *    arg2 is an existing directory with writing permission
 */
static void validateArguments(char *pageDirectory, char *indexFilename)
{
    // validate pageDirectory is crawler produced directory
    if (!isCrawlerDirectory(pageDirectory)){
        fprintf(stderr, "argument 1: '%s' is not a a crawler produced directory\n", pageDirectory);
        exit(2);
    }

    // validate indexFileName is a pathname for an existing directory
    FILE *fp = fopen(indexFilename, "w");
	if (fp == NULL) {
		fprintf(stderr, "argument 2: '%s' file name does not exist or is not writable\n", indexFilename);
        exit(3);
	}
    fclose(fp);
}

/**************** indexBuild() ****************/
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
static index_t *indexBuild(char *pageDirectory)
{
    index_t *index = assertp(indexNew(200), "new index");

    webpage_t *loadedWebpage;
    int docID = 1; 

    // loadPage return null when the file doesn't exist
    while ((loadedWebpage = loadPage(pageDirectory, docID)) != NULL){ 
        indexPage(loadedWebpage, index, docID);
        webpage_delete(loadedWebpage);
        docID++; // read the next file
    }
    return index;
}

/**************** indexPage() ****************/
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
static void indexPage(webpage_t *page, index_t *index, int docID)
{
    char *nextWord;
    int pos = 0; 
    while ((nextWord = webpage_getNextWord(page, &pos)) != NULL){
        if (strlen(nextWord) < 3){
            free(nextWord); 
            continue;
        }

        // normalize word (convert to lowercase if the word has 3 or more characters)
        normalizeWord(nextWord);

        // check whether the key(word) exists
        counters_t *ctrs;
        if (indexFind(index, nextWord) == NULL){ 
            ctrs = assertp(counters_new(), "new ctrs in indexPage");
            counters_add(ctrs, docID); 
            indexInsert(index, nextWord, ctrs);
            free(nextWord); // free memory b/c set makes a new copy of the key
            continue;
        }

        // store the counters if it already exist
        ctrs = indexFind(index, nextWord);
        int count;

        // since the word is already in the index, free the memory
        free(nextWord);

        // check whether docID exists as key in ctrs
        if ((count = counters_get(ctrs, docID)) == 0){
            // docID does not exist -> add docID
            counters_add(ctrs, docID);
            continue;
        }

        // docID exists, so increment the count
        counters_set(ctrs, docID, count+1);
    }
}