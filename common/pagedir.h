/* 
 * pagedir.h - header file for common page directory module used by crawler, indexer, and querier 
 *
 * Aimen Abdulaziz, Spring 2022
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdbool.h>
#include "webpage.h"

/**************** isDirectory **************************/
/* If character pointer pageDir is NULL, print error message to stderr and return false,
 *
 * We return:
 *   true - if the pathname provided as pageDirectory is indeed a directory, and is writable. 
 *   otherwise, return false
 * We exit if p=NULL, after printing message to stderr.
 */
bool isDirectory (char *pageDir);

/**************** pageSaver **************************/
/*
 * output a page to the appropriate file 
 * make a copy of the user provided fname and concatenate the '/fileID' or 'fileID' at the end
 * handle the case where a user might provide have '/' at the end of the directory name
 */
void pageSaver(webpage_t *page, const char *fDirectory, int fileID);

/**************** isCrawlerDirectory **************************/
/* Verify whether dir is indeed a Crawler-produced directory 
 * Return:
 *   true, if dir has ./crawler file saved while crawling
 *   otherwise, return false if dir is not a valid pathname, not a directory, 
 *   not a readable directory, not a Crawler produced directory, or some other error
 */
bool isCrawlerDirectory(char *dir);

/**************** loadPage ****************/
/* Read a page from crawler created files 
 * Re-create the webpage structure by reading the file and return the webpage
 * Caller is responsible for:
 *   later calling webpage_delete();
 */
webpage_t *loadPage(char *fDirectory, int fileID);

#endif
