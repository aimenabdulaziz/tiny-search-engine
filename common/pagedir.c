/* 
 * pagedir.c -  common functions shared between crawler, indexer, and querier
 *
 * see pagedir.h for more information.
 *
 * Aimen Abdulaziz, Spring 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "memory.h"
#include "webpage.h"
#include "file.h"

/**************** isDirectory() ****************/
/* see pagedir.h for description */
bool isDirectory (char *pageDir)
{
    if (pageDir == NULL){
        fprintf(stderr, "page directory is null\n");
        return false;
    }

    // allocate memory for file name  
    char fileName[strlen(pageDir) + 10]; // +10 for concatenating '/.crawler'
    strcpy(fileName, pageDir);

    // concatenate the new file name according to the user input
    if (pageDir[strlen(pageDir) - 1] == '/'){
        strcat(fileName, ".crawler"); // concatenate file name
    }
    else{
        strcat(fileName, "/.crawler"); // concatenate file name
    }
   
    // open the file if it exist and write a new file there
    FILE *fp;
    if ((fp = fopen(fileName, "w")) == NULL){ 
        return false;
    }

    // file open is not null -> valid directory
    fclose(fp);
    return true;
}

/**************** pageSaver() ****************/
/* see pagedir.h for description */
void pageSaver(webpage_t *page, const char *fDirectory, int fileID)
{
    // make a copy of the fileDirectory to add file ID
    char fileDirectory[strlen(fDirectory)+15];
    if (fDirectory[strlen(fDirectory) - 1] == '/'){
        snprintf(fileDirectory, strlen(fDirectory)+15, "%s%d", fDirectory, fileID);
    }
    else{
        snprintf(fileDirectory, strlen(fDirectory)+15, "%s/%d", fDirectory, fileID);
    }

    // open file 
    FILE *fp = fopen(fileDirectory, "w");
    assertp(fp, "pageSaver cannot open file for writing\n"); // make sure fp is not NULL

    // write to the file and close it afterwards
    fprintf(fp, "%s\n%d\n%s", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));
    fclose(fp);
}

/**************** isCrawlerDirectory() ****************/
/* see pagedir.h for description */
bool isCrawlerDirectory(char *dir)
{
    if (dir == NULL){
    fprintf(stderr, "page directory is null\n");
    return false;
    }

    char validateDirectory[strlen(dir) + 10]; // +10 for '\0' and '/.crawler'
    strcpy(validateDirectory, dir); // copy dir to validateDirectory
    
    // concatenate ./crawler to validateDirectory accordingly 
    if (validateDirectory[strlen(validateDirectory) - 1] == '/'){
        strcat(validateDirectory, ".crawler"); 
    }
    else{
        strcat(validateDirectory, "/.crawler");
    }

    // open the file for reading 
    FILE *fp;
    if ((fp = fopen(validateDirectory, "r")) == NULL){ 
        return false;
    }

    // file open is not null -> valid directory
    fclose(fp);
    return true;
}

webpage_t *loadPage(char *fDirectory, int fileID)
{
    // make a copy of the fileDirectory to add file ID
    char fileDirectory[strlen(fDirectory)+15];
    if (fDirectory[strlen(fDirectory) - 1] == '/'){
        snprintf(fileDirectory, strlen(fDirectory)+15, "%s%d", fDirectory, fileID);
    }
    else{
        snprintf(fileDirectory, strlen(fDirectory)+15, "%s/%d", fDirectory, fileID);
    }

    // open file 
    FILE *fp = fopen(fileDirectory, "r");
    if (fp == NULL){
        return NULL; // stop reading as the file doesn't exist 
    }

    char *URL = freadlinep(fp); // read first line
    char *depthRead = freadlinep(fp);
    int depth = atoi(depthRead); // read second line
    free(depthRead); // free this as webpage_delete won't free this later
    char *html = freadfilep(fp); // read the remaining of the file

    // close file
    fclose(fp);

    // create a new webpage based on the data read from the file
    webpage_t *loadedWebpage = assertp(webpage_new(URL, depth, html), "new reloaded webpage");

    return loadedWebpage;
}
