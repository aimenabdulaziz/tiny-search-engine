/* 
 * indextest.c - loads an index file produced by the indexer and saves it to another file.
 *
 * usage: ./indextest oldIndexFilename newIndexFilename
 *
 * 
 * Aimen Abdulaziz, Spring 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include "index.h"

/********************** local function prototypes ********************* */
static void validateArguments(int argc, char *oldIndexFilename, char *newIndexFilename);

/* ******************** main ******************** */
int main(int argc, char *argv[])
{   
    // parse arguments
    char *oldIndexFilename = argv[1]; 
    char *newIndexFilename = argv[2]; 

    validateArguments(argc, oldIndexFilename, newIndexFilename);

    // load page from old and save to new index file
    index_t *index = indexLoad(oldIndexFilename);
    indexSave(index, newIndexFilename);
    indexDelete(index);
    return 0;
}


static void validateArguments(int argc, char *oldIndexFilename, char *newIndexFilename)
{
    // validate number of arguments
    if (argc != 3){
        fprintf(stderr, "usage: ./indextest oldIndexFilename newIndexFilename\n");
        exit(11);
    }

    // validate oldIndexFilename is readable
    FILE *fp = fopen(oldIndexFilename, "r");
	if (fp == NULL) {
		fprintf(stderr, "argument 1: '%s' is not readable\n", oldIndexFilename);
        exit(12);
	}
    fclose(fp);

    // validate newIndexFilename is writable
    fp = fopen(newIndexFilename, "w");
	if (fp == NULL) {
		fprintf(stderr, "argument 2: '%s' file name does not exist or is not writable\n", newIndexFilename);
        exit(13);
	}
    fclose(fp);
}
