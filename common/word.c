/* 
 * word.c -  common word module used by Indexer and Querier
 *
 * see word.h for more information.
 *
 * Aimen Abdulaziz, Spring 2022
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**************** normalizeWord() ****************/
/* see word.h for description */
char *normalizeWord(char *word)
{
    if (word != NULL){
        for (int i = 0; i < strlen(word); i++){
            word[i] = tolower(word[i]);
        }
        return word;
    }
    return NULL;
}
