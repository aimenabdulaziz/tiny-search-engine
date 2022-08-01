/* 
 * crawler.c -  crawls a website and retrieves webpages starting with a specified URL
 *
 * usage: ./crawler seedURL pageDirectory maxDepth
 * 
 * see .md files for further details
 * 
 * Aimen Abdulaziz, Spring 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webpage.h"
#include "pagedir.h"
#include "memory.h"
#include "bag.h"
#include "hashtable.h"

/********************** local function prototypes ********************* */
static void validateArguments(char *URL, char *pageDirectory, int maxDepth);
static int crawler(char *URL, char *pageDirectory, int maxDepth);
static void pageScanner(webpage_t *page, bag_t *toVisit, hashtable_t *seen, int maxDepth);
inline static void logr(const char *word, const char *url, const int depth);

/* ******************** main ******************** */
/* parse arguments and call other methods */
int main(const int argc, char *argv[])
{
    int exitStatus = 0;

    // validate number of arguments
    char *program = argv[0];
    if (argc != 4){
        fprintf(stderr, "usage: %s seedURL pageDirectory maxDepth\n", program);
        exit(1);
    }

    // parse arguments
    char *URL = argv[1];
    char *pageDirectory = argv[2];
    
    // validate maxDepth is a valid integer
    int maxDepth;
    char excess;
    if (sscanf(argv[3], "%d%c", &maxDepth, &excess) != 1){
        fprintf(stderr, "argument 3: '%s' is not a valid integer\n", argv[3]);
        exit(2);
    }

    // call validateArgument helper method 
    validateArguments(URL, pageDirectory, maxDepth);

    // call crawler helper method to loop through URLs and call pageSaver and pageScanner   
    crawler(URL, pageDirectory, maxDepth);

    return exitStatus;
}

/**************** validateArguments() ****************/
/* helper function to parse and validates the arguments */
static void validateArguments(char *URL, char *pageDirectory, int maxDepth)
{
    // validate seedURL is an internal URL
    if (!IsInternalURL(URL)){
        fprintf(stderr, "argument 1: '%s' is not a valid internal URL\n", URL);
        exit(3);
    }

    // validate pageDirectory is a pathname for an existing directory
    if (!isDirectory(pageDirectory)){
        fprintf(stderr, "argument 2: '%s' file name does not exist or is not writeable\n", pageDirectory);
        exit(4);
    }
    
    // validate maxDepth is in range 0...10
    if (maxDepth < 0 || maxDepth > 10){
        fprintf(stderr, "argument 3: '%d' max depth must be greater than 0 and less than 10\n", maxDepth);
        exit(5);
    }
}

/**************** crawler() ****************/
/* loops over all pages to explore, until the list is exhausted */
static int crawler(char *URL, char *pageDirectory, int maxDepth)
{
    // make a webpage for the seedURL, marked with depth=0
    char *seedURL = assertp(malloc(strlen(URL)+1), "url copy");
    strcpy(seedURL, URL);
    webpage_t *seedWebpage = assertp(webpage_new(seedURL, 0, NULL), "new seed webpage"); // depth for seed URL is 0

    // make a new bag and hashtable
    bag_t *toCrawlBag = assertp(bag_new(), "new bag"); // bag of pages we have yet to explore
    hashtable_t *htSeen = assertp(hashtable_new(200), "new hashtable"); // hashtable of URLs we've seen so far

    // insert seedPage to bag and seedURL hashtable 
    bag_insert(toCrawlBag, seedWebpage); 
    hashtable_insert(htSeen, seedURL, ""); 

    // keep extracting a page from the bag until it is exhausted
    webpage_t *currentWebpage; // store webpage when extracted from the toCrawl bag
    int fileID = 1;
    while ((currentWebpage = bag_extract(toCrawlBag)) != NULL){
        if (webpage_fetch(currentWebpage)) { 
            logr("Fetched", webpage_getURL(currentWebpage), webpage_getDepth(currentWebpage));
            pageSaver(currentWebpage, pageDirectory, fileID);
            pageScanner(currentWebpage, toCrawlBag, htSeen, maxDepth);
            fileID++;
	    }
	    else { // failed to fetch the page
            fprintf(stderr, "failed to fetch %s\n", webpage_getURL(currentWebpage));
	    }
        webpage_delete(currentWebpage); // clear currentWebPage
    }

    // free allocated memory
    bag_delete(toCrawlBag, webpage_delete);
    hashtable_delete(htSeen, NULL); // hashtable will delete the keys
    return 0;
}

/**************** pageScanner() ****************/
/* extracts URLs from a page and processes each one */
static void pageScanner(webpage_t *page, bag_t *toVisit, hashtable_t *seen, int maxDepth)
{
    char *nextURL;
    int pos = 0;

    // extracts URLs from a page until current depth is == max depth
    if (webpage_getDepth(page) < maxDepth){ // parent (current) depth is less than maxDepth
        logr("Scanning", webpage_getURL(page), webpage_getDepth(page));
        while ((nextURL = webpage_getNextURL(page, &pos)) != NULL){
            logr("Found", nextURL, webpage_getDepth(page));
            
            // confirm the URL is internal
            if (IsInternalURL(nextURL)){  
                if (hashtable_insert(seen, nextURL, "")){ // hashtable return false for duplicate keys
                    webpage_t *nextPage = assertp(webpage_new(nextURL, webpage_getDepth(page)+1, NULL), "new webpage"); // create a new webpage
                    bag_insert(toVisit, nextPage);
                    logr("Added", nextURL, webpage_getDepth(page));
                }
                else{
                    logr("IgnDupl", nextURL, webpage_getDepth(page));
                    free(nextURL);
                }
            }
            else{
                logr("IgnExtrn", nextURL, webpage_getDepth(page));
                free(nextURL);
            }
        }
    }
}

/**************** logr() ****************/
/* log one word (1-9 chars) about a given url */
inline static void logr(const char *word, const char *url, const int depth)
{
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}
