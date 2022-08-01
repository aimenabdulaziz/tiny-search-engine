/* 
 * index.h - header file for CS50 index module
 *
 * Index includes a suite of functions that implement the “index” data structure; 
 * this module includes functions to write an index to a file 
 * (used by indexer) and read an index from a file (used by querier).
 * 
 * Aimen Abdulaziz, Spring 2022
 */

#ifndef __INDEX_H__
#define __INDEX_H__

#include <stdio.h>
#include <stdbool.h>
#include "counters.h"
#include "hashtable.h"

/**************** global types ****************/
/* 
 * index_t is a hashtable of countersets; it is perfect to map 
 * from a word to a list of (docID, count) pair 
 * index_t is basically another name for hashtable_t
 */
typedef hashtable_t index_t; // opaque to users of the module

/**************** functions ****************/

/**************** indexNew ****************/
/* Create a new (empty) index -- hashtable.
 *
 * Caller provides:
 *   number of slots to be used for the index (must be > 0).
 * We return:
 *   pointer to the new index; return NULL if error.
 * We guarantee:
 *   index is initialized empty.
 * Caller is responsible for:
 *   later calling index_delete.
 */
index_t *indexNew(int num_slots);

/**************** indexInsert ****************/
/* Insert item, identified by key (string), into the given index_t.
 *
 * Caller provides:
 *   valid pointer to hashtable, valid string for key, valid pointer for item.
 * We return:
 *   false if key exists in index, any parameter is NULL, or error;
 *   true if new item was inserted.
 * Notes:
 *   The key string is copied for use by the index module; that is, the module
 *   is responsible for allocating memory for a copy of the key string, and
 *   later deallocating that memory; thus, the caller is free to re-use or 
 *   deallocate its key string after this call.  
 */
bool indexInsert(index_t *index, const char *key, counters_t *ctrs);

/**************** indexFind ****************/
/* Return the item (counters_t) associated with the given key.
 *
 * Caller provides:
 *   valid pointer to index, valid string for key.
 * We return:
 *   pointer to the item corresponding to the given key, if found;
 *   NULL if hashtable is NULL, key is NULL, or key is not found.
 * Notes:
 *   the index is unchanged by this operation.
 *   caller can use counters_get(ctrs, key) where key is fileID to find the word frequency
 */
counters_t *indexFind(index_t *index, const char *key);

/**************** indexDelete ****************/
/* Delete index, calling a delete function on each item.
 *
 * Caller provides:
 *   valid index pointer,
 * We do:
 *   if index==NULL, do nothing.
 *   otherwise, call counters_delete on each item
 *   since the index_t is hashtable of countersets, we pass counters_delete to delete the item
 *  
 * Notes:
 *   We free the strings that represent key for each item, because 
 *   this module allocated that memory in hashtable_insert.
 */
void indexDelete(index_t *index);

/**************** indexSave ****************/
/* Output format: key[word] countersKey[fileID] countersValue[frequency] 
 * index - hashtable_t/index_t with the data
 * index_file - path location for saving the file
 */
void indexSave(index_t *index, char *index_file);

/**************** indexLoad ****************/
/* Load item from an index_file 
 * If index_file==NULL, print error message and exit.
 * Caller is responsible for:
 *   later calling index_delete.
 */
index_t* indexLoad(char* indexFile);

#endif
