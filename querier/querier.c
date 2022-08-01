/* 
 * querier.c -  reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. 
 *
 * usage: ./querier pageDirectory indexFilename
 * 
 * see .md files for further details
 * 
 * Aimen Abdulaziz, Spring 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h> 
#include <stdbool.h>
#include "pagedir.h"
#include "index.h"
#include "file.h"
#include "word.h"
#include "memory.h"
#include "counters.h"
#include "webpage.h"

/******** local data types *******/
typedef struct twoCtrs{
	counters_t *result;
	counters_t *another;
} twoCtrs_t;

typedef struct dataArray{
    int **scoreArr; // array of multiple key,count pairs
    int index; // current index
} dataArray_t;

/********************** local function prototypes ********************* */
static void validateArguments(const int argc, char *argv[], char **pageDirectory, char **indexFilename);
static void handleInput(index_t *index, char*pageDirectory);
static bool hasSyntaxError(char *words[], int numWords, bool first, bool last);
static bool compareStrings(char *words[], char *str1, char *str2, int i);
int fileno(FILE *stream);
static void prompt(void);
static void printQuery(char *words[], int numWords);
static void processQuery(index_t *index, char *words[], int numWords, char *pageDirectory);
static void countersIntersect(counters_t* ct1, counters_t* ct2);
static void intersectHelper(void *arg, const int key, const int count);
static inline int min(const int a, const int b);
void countersMerge(counters_t* ct1, counters_t* ct2);
static void mergeHelperFirst(void *arg, const int key, const int count);
static void mergeHelperSecond(void *arg, const int key, const int count);
static void printResult(counters_t *ctrsAccumulator, char *pageDirectory);
static void ctrsItemCount(void *arg, const int key, const int count);
static void nonZeroItemCount(void *arg, const int key, const int count);
static int** makeArray(counters_t *ctrs, int numElements);
static void makeArrayHelper(void *arg, const int key, const int count);
static int comparator (const void*, const void*);
static void getURL(char *pageDirectory, int fileID, char **URL);
static void freeKeyCountPairs(int **keyCountPairs, int numElements);

/* ******************** main ******************** */
/* Parse arguments and call other methods */
int main(const int argc, char *argv[])
{
    // initialize pointers
    char *pageDirectory = NULL; // pathname of crawler produced directory
    char *indexFilename = NULL; // indexer produced index file

    // parse and validate arguments
    validateArguments(argc, argv, &pageDirectory, &indexFilename);

    // load the index into an internal data structure
    index_t *index = assertp(indexLoad(indexFilename), "Error loading index");

    // parse query according to specs and call processQuery for each valid query
    handleInput(index, pageDirectory);

    // clean up
    indexDelete(index);
    return 0;
}

/**************** validateArguments() ****************/
/* Helper function to validate the arguments 
 * 
 * Validate:
 *    arg1 is the pathname of a directory produced by the Crawler
 *    arg2 is the pathname of a file produced by the Indexer (readable file)
 * 
 * Function exits with an error code if there is any problem in the verification process
 */
static void validateArguments(const int argc, char *argv[], char **pageDirectory, char **indexFilename)
{
    // validate number of arguments
    char *program = argv[0];
    if (argc != 3){
        fprintf(stderr, "usage: %s pageDirectory indexFilename\n", program);
        exit(1);
    }

    // validate pageDirectory is crawler produced directory
    *pageDirectory = argv[1];
    if (!isCrawlerDirectory(*pageDirectory)){
        fprintf(stderr, "argument 1: '%s' is not a a crawler produced directory\n", *pageDirectory);
        exit(2);
    }

    // validate indexFileName is an index file
    *indexFilename = argv[2];
    FILE *fp = fopen(*indexFilename, "r");
    if (fp == NULL) {
        fprintf(stderr, "argument 2: '%s' file name does not exist or is not readable\n", *indexFilename);
        exit(3);
    }
    fclose(fp);
}

/**************** handleInput() ****************/
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
static void handleInput(index_t *index, char*pageDirectory)
{
    // read user input from stdin until EOF
    char *line, *token;
    prompt(); // prompt user for stdin
    while ((line = freadlinep(stdin)) != NULL){
        char* words[strlen(line)]; // pointer to an array of strings
        int i = 0; // index for inserting tokens in words array
        bool illegalChar = false; // keeps track the detection of illegal character -> will reset for every line
        bool first = true; // helpful to keep track of the first word
        bool last = false; // updated to true when the last word is added to the array
        
        // get the first token 
        char delimit[] = " \t\n"; // delimit by white space, tab, and new line
        token = strtok(line, delimit);

        // continue tokenizing until EOF
        while (token != NULL) {
            // ensure all the characters are legal (alphabets)
            for (int j = 0; j < strlen(token); j++){
                if (!isalpha(token[j]) || token[j] == '\t'){
                    fprintf(stdout, "Error: bad character '%c' in query.\n", token[j]);
                    illegalChar = true;
                    break;
                }
            }

            // add the token to the words array
            normalizeWord(token);
            words[i] = token;
            i++; // increment index

            // verify the first word doesn't have syntax error before proceeding further
            if (illegalChar || hasSyntaxError(words, i, first, last)){
                illegalChar = true;
                break;
            }

            token = strtok(NULL, delimit); // next token
            first = false; // has already checked the first word
        }

        // check if illegal character has been detected or the last word has a syntax error 
        last = true;
        if (illegalChar || hasSyntaxError(words, i, first, last)){
            free(line);
            prompt(); 
            continue; // skip the line
        }

        // print a clean query in the stdout and process the query
        printQuery(words, i);
        if (i == 0){
            prompt(); 
            continue;
        }
        processQuery(index, words, i, pageDirectory);
        free(line);
        prompt(); 
    }
}

/**************** hasSyntaxError() ****************/
/* Helper function to perform extensive tests based on the specs to verify if the input has syntax error
 * Return boolean:
 *      true, if:
 *          'and' / 'or' appear at the beginning or end of a query
 *          'and' & 'or' appear consecutively 
 *      otherwise, false
 */
static bool hasSyntaxError(char *words[], int numWords, bool first, bool last)
{   
    // check if the first word is 'and' / 'or'
    if (first){
        if (strcmp(words[0], "and") == 0 || strcmp(words[0], "or") == 0 ){
            printQuery(words, numWords);
            fprintf(stdout, "Error: '%s' cannot be first\n", words[0]);
            return true; // first word with syntax error
        }
        return false; // first word with valid syntax
    }

    // check for adjacent or consecutive 'and' / 'or'
    for (int i = 0; i < numWords-1; i++){
        // check for adjacent 
        if (compareStrings(words, "or", "or", i) || compareStrings(words, "and", "and", i)){
            printQuery(words, numWords);
            fprintf(stdout, "Error: '%s' cannot be adjacent with each other\n", words[i]);
            return true;
        }

        if (compareStrings(words, "and", "or", i) || compareStrings(words, "or", "and", i)){
            printQuery(words, numWords);
            fprintf(stdout, "Error: '%s' and '%s' cannot appear consecutive to each other\n", words[i], words[i+1]);
            return true;
        }
    }

    // check if the last word is 'and' / 'or'
    if (last){
        if (strcmp(words[numWords-1], "and") == 0 || strcmp(words[numWords-1], "or") == 0 ){
            printQuery(words, numWords);
            fprintf(stdout, "Error: '%s' cannot be last\n", words[numWords-1]);
            return true; // first word with syntax error
        }
    }
    return false;
}

/**************** compareStrings() ****************/
/* Helper function for validating querier by comparing two consecutive words in an array with two strings
 * Return:
 *      true, if the str1 is equal to the first word and str2 is equal to the second word
 *      otherwise, false
 */
static bool compareStrings(char *words[], char *str1, char *str2, int i)
{
    if (strcmp(words[i], str1) == 0 && strcmp(words[i+1], str2) == 0){
        return true;
    }
    return false;
}

/**************** prompt() ****************/
/* print a prompt for an interactive user */
static void prompt(void)
{
  // print a prompt if stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

/**************** printQuery() ****************/
/* Loop over all the words in the array and print a clean query
 * This is the same query that will be processed by the Tiny Search Engine
 */
static void printQuery(char *words[], int numWords)
{
    fprintf(stdout, "Query:");
    for (int i = 0; i < numWords; i++){
        fprintf(stdout, " %s", words[i]);
    }
    fprintf(stdout, "\n");
}

/**************** processQuery() ****************/
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
{
    // make an accumulator (result) counters
    counters_t *ctrsAccumulator = assertp(counters_new(), "Error initializing result counters");
    counters_t *ctrsTemp = NULL; // points to the current 'and' cluster; initially NULL
    bool reset = true;
    bool newWord = true; // changed to false after visiting the first word in the 'and' cluster

    for (int i = 0; i < numWords; i++){
        if (reset){
            ctrsTemp = assertp(counters_new(), "Error initializing a temp counters");
            reset = false;
        }

        // merge temp with the result and free temp to start accumulating 'and' cluster again
        if (strcmp(words[i], "or") == 0){
            // find the union of the two ctrs
            countersMerge(ctrsAccumulator, ctrsTemp);
            counters_delete(ctrsTemp);
            reset = true;
            newWord = true;
        }

        // skip if the word is 'and'
        else if (strcmp(words[i], "and") != 0){
            // search the word in the index
            counters_t *ctrsFound = indexFind(index, words[i]);
                
            // when ctrsTemp is empty, copy everything in ctrsFound to ctrsTemp
            if (newWord){
                countersMerge(ctrsTemp, ctrsFound); 
                newWord = false;
            }

            // add the intersection of ctrsFound to 'and' cluster accumulator (ctrsTemp)
            else{
                countersIntersect(ctrsTemp, ctrsFound);
            }
        }
    }
    countersMerge(ctrsAccumulator, ctrsTemp);
    printResult(ctrsAccumulator, pageDirectory);
    
    // free counters
    counters_delete(ctrsTemp);
    counters_delete(ctrsAccumulator);
}

/**************** countersIntersect() ****************/
/* Invoked when handling andSequence
 * For two counters,
 *      initialize a struct that holds two ctrs where the first ctrs will be treated as the result ctrs
 *      iterate through ctr1 and pass the struct hold two ctrs and intersectHelper
 */
void countersIntersect(counters_t* ct1, counters_t* ct2)
{   
    bool newCtr1 = false;
    bool newCtr2 = false;
    if (ct1 == NULL){
        ct1 = assertp(counters_new(), "Error initializing ctrs for NULL c1");
        newCtr1 = true;
    }

    if (ct2 == NULL){
        // used when ctrsFound is NULL 
        ct2 = assertp(counters_new(), "Error initializing ctrs for NULL c2");
        newCtr2 = true;
    }

    if (ct1 != NULL && ct2 != NULL){
        // leverage the twoCtrs data type created earlier to compute the intersection of the two ctrs
        twoCtrs_t args = {ct1, ct2}; 
        counters_iterate(ct1, &args, intersectHelper);
    }

    // free memory if new ctrs was created in this method
    if (newCtr1){
        counters_delete(ct1);
    }

    if (newCtr2){
        counters_delete(ct2);
    }
}

/**************** intersectHelper() ****************/
/* Helper function for coutersIntersect
 * If the ctrs have the same key, 
 *      store the min count for the specific key as count in result ctrs
 * Else,
 *      store 0 as the count in the result ctrs 
 */
static void intersectHelper(void *arg, const int key, const int count)
{
	twoCtrs_t *two = arg; 
	counters_set(two->result, key, min(count, counters_get(two->another, key)));	
}

/**************** min() ****************/
/* Given two integers, it will return the smallest one */
static inline int min(const int a, const int b) 
{
  return (a < b ? a : b); // return a if a < b; otherwise, return b
}

/**************** countersMerge() ****************/
/* Merge two ctrs
 * If either of the two ctrs is NULL,
 *      don't do anything
 * Else,
 *      iterate through each and store the result in the first ctrs
 */
void countersMerge(counters_t* ct1, counters_t* ct2)
{   
    if (ct1 != NULL && ct2 != NULL){
        // leverage the twoCtrs data type created earlier to compute the intersection of the two ctrs
        twoCtrs_t args = {ct1, ct2}; 
        counters_iterate(ct1, &args, mergeHelperFirst); // iterate through the keys of ct1 and merge with ct2
        counters_iterate(ct2, &args, mergeHelperSecond); // iterate through the keys of ct2 and merge with ct1
    }
}
/**************** mergeHelperFirst() ****************/
/* Iterate through the keys of ct1 and merge with ct2 */
static void mergeHelperFirst(void *arg, const int key, const int count)
{
    twoCtrs_t *two = arg;
    counters_set(two->result, key, counters_get(two->another, key) + count); // sum the counts to merge the counters
}

/**************** mergeHelperSecond() ****************/
/* Iterate through the keys of ct2 and merge with ct1 */
static void mergeHelperSecond(void *arg, const int key, const int count)
{
    twoCtrs_t *two = arg;
    if (counters_get(two->result, key) == 0){
        counters_set(two->result, key, counters_get(two->result, key) + count); // sum the counts to merge the counters
    }
}

/**************** printResult() ****************/
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
{
    int numElements = 0; // total number of key,count pairs
    int nonZeroCount = 0; // total number of key,count pairs with non-zero count value
    counters_iterate(ctrsAccumulator, &numElements, ctrsItemCount);
    counters_iterate(ctrsAccumulator, &nonZeroCount, nonZeroItemCount);

    if (numElements == 0 || nonZeroCount == 0){
        fprintf(stdout, "No documents match.\n");
    }

    else{
        printf("Matches %d documents (ranked):\n", nonZeroCount);

        // make nested array where each index stores key,value pairs
        int **keyCountPairs = makeArray(ctrsAccumulator, numElements);

        // sort in decreasing order of count
        qsort(keyCountPairs, numElements, sizeof(int[2]), comparator);

        // print the set of documents in the query result
        for (int i = 0; i < numElements; i++){
            char *URL;
            int docKey = keyCountPairs[i][0]; 
            int docScore = keyCountPairs[i][1];

            // don't do anything if the count for the specific key is 0
            if (docScore == 0){ 
                continue;
            }

            // obtain the URL by reading the first line of the relevant document file
            getURL(pageDirectory, docKey, &URL);

            // print the score, docID, and URL in a user readable format
            printf("score\t%d doc\t%d: %s\n", docScore, docKey, URL);
            free(URL);
        }
        freeKeyCountPairs(keyCountPairs, numElements);
    }
    // iterate through the counters and print the docs (URL)
    fprintf(stdout, "-----------------------------------------------\n");
}

/**************** ctrsItemCount() ****************/
/* count the non-null items in the counters */
static void ctrsItemCount(void *arg, const int key, const int count)
{
  int *nkeys = arg;
  if (nkeys != NULL)
    (*nkeys)++;
}

/**************** nonZeroItemCount() ****************/
/* total number of key,count pairs with non-zero count value */
static void nonZeroItemCount(void *arg, const int key, const int count)
{
  int *nkeys = arg;
  if (nkeys != NULL && count != 0)
    (*nkeys)++;
}

/**************** makeArray() ****************/
/* Create and return a nested array where each index in the array is a key,count pair
 * This array is later passed to qsort for ranking in decreasing 
 * User is responsible for later calling freeKeyCountPairs
 */
static int **makeArray(counters_t *ctrs, int numElements)
{
    // nested array to hold key,count pairs
    dataArray_t *dataArray = assertp(malloc(sizeof(dataArray_t)), "null data array");
    dataArray->index = 0;
    dataArray->scoreArr = assertp(malloc(numElements * sizeof(int*)), "null key-count pairs");
    int **keyCountPairs = dataArray->scoreArr;

    for (int i = 0; i < numElements; i++){
        // initialize the inner array for each index 
        keyCountPairs[i] = assertp(calloc(2, sizeof(int)), "null");
    }

    counters_iterate(ctrs, dataArray, makeArrayHelper); // args is a pointer to first element in the list
    free(dataArray);
    return keyCountPairs;
}

/**************** makeArrayHelper() ****************/
/* Helper function for makeArray
 * In each index,
 *      add key at idx 0 and count at idx 1
 * Increment the current index while adding in the nested array
 */
static void makeArrayHelper(void *arg, const int key, const int count)
{
    // cast arg to dataArray type
    dataArray_t *dataArray = arg; 
    int **keyCountPairs = dataArray->scoreArr;

    // insert in the array of key,value pairs
    int i = dataArray->index; // index
    keyCountPairs[i][0] = key;
    keyCountPairs[i][1] = count;

    dataArray->index += 1; // increment current index
}

/**************** comparator() ****************/
/* Comparator passed to qsort -> follows the format qsort takes 
 * Designed to sort in decreasing order of scores
 */
static int comparator(const void* a, const void* b)
{
    const int *arg1 = *(const int**)a;
    const int *arg2 = *(const int**)b;
    return arg2[1] - arg1[1]; // compare the scores
}

/**************** getURL() ****************/
/* Obtain the URL by reading the first line of the relevant document file from the pageDirectory */
static void getURL(char *pageDirectory, int fileID, char **URL)
{
    // make a copy of the fileDirectory to add file ID
    char fileDirectory[strlen(pageDirectory)+15];
    if (pageDirectory[strlen(pageDirectory) - 1] == '/'){
        snprintf(fileDirectory, strlen(pageDirectory)+15, "%s%d", pageDirectory, fileID);
    }
    else{
        snprintf(fileDirectory, strlen(pageDirectory)+15, "%s/%d", pageDirectory, fileID);
    }

    // open file 
    FILE *fp = assertp(fopen(fileDirectory, "r"), "error opening index file\n");
    *URL = freadlinep(fp); // read first line
    fclose(fp);
}

/**************** validateArguments() ****************/
/*
 *
 */
// free all the memory assigned earlier
static void freeKeyCountPairs(int **keyCountPairs, int numElements)
{
    for (int i = 0; i < numElements; i++){
        free(keyCountPairs[i]);
    }
    free(keyCountPairs);
}