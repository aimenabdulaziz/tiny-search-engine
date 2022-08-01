/* 
 * index.c -  common index module
 *
 * see index.h for more information.
 *
 * Aimen Abdulaziz, Spring 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "index.h"
#include "counters.h"
#include "file.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/

/**************** global functions ****************/
/* that is, visible outside this file */

/* see index.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static void saveItem(void *arg, const char *key, void *item);
static void saveCounters (void *arg, const int key, int count);
static void deleteCounters(void* data);

/**************** indexNew() ****************/
/* see index.h for description */
index_t *indexNew(int num_slots)
{
    return assertp(hashtable_new(300), "new hashtable");
}

/**************** indexInsert() ****************/
/* see index.h for description */
bool indexInsert(index_t *index, const char *key, counters_t *ctrs)
{
    return hashtable_insert(index, key, ctrs);
}

/**************** indexFind() ****************/
/* see index.h for description */
counters_t *indexFind(index_t *index, const char *key)
{
    return hashtable_find(index, key);
}

/**************** indexDelete() ****************/
/* see index.h for description */
void indexDelete(index_t *index)
{
    hashtable_delete(index, deleteCounters);
}

/**************** deleteCounters() ****************/
/* helper function to delete counters */
static void deleteCounters(void* data) 
{
   counters_t *ctrs = data; // specify the data type
   counters_delete(ctrs);
 }

/**************** indexSave() ****************/
/* see index.h for description */
void indexSave(index_t *index, char *index_file)
{
	// save index content to an external file 
	FILE *fp = assertp(fopen(index_file, "w"), "Error opening index_file");
	hashtable_iterate(index, fp, saveItem);
	fclose(fp);
}

/**************** saveItem() ****************/
/*
 * helper function to save item in index 
 * leverage save_counters to write the output in the fp* provided by the caller
 * output format: key[word] countersKey[fileID] countersValue[frequency]
 */
static void saveItem(void *arg, const char *key, void *item)
{
	FILE *fp = arg; 
	counters_t *ctrs = item; // cast to counter_t
	fprintf(fp, "%s", key); 
	counters_iterate(ctrs, fp, saveCounters);
	fprintf(fp, "\n"); // inserts new line 
} 

/**************** saveCounters() ****************/
/* helper function to save items in counters */
static void saveCounters(void *arg, const int key, int count)
{
	FILE *fp = arg; 
	fprintf(fp, " %d %d", key, count);
}

/**************** indexLoad() ****************/
/* see index.h for description */
index_t* indexLoad(char* indexFile)
{
	FILE *fp = fopen(indexFile, "r");
    assertp(fp, "index_s load cannot open file for reading\n"); // make sure fp is not NULL
   	
	// use the number of lines in the file to assign slots for index_t
	int slots = (lines_in_file(fp) * 100) / 70;  // multiply by 100/70 so that the file size is 70% of the number of slots in the index_t
	index_t *index = assertp(indexNew(slots), "new index"); 
	
	// read index_file and re-create index
	char *word; // key 
	while ((word = freadwordp(fp)) != NULL){
		counters_t *ctrs = assertp(counters_new(), "new counters");
		int fileID;
		int count;
		while (fscanf(fp ,"%d %d ", &fileID, &count) == 2){
			counters_set(ctrs, fileID, count);
		}
		indexInsert(index, word, ctrs);
		free(word);
	}
	fclose(fp);
	return index;
}
