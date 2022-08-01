/* 
 * word.h - Implements NormalizeWord which is used by Indexer and Querier
 * 
 * Aimen Abdulaziz, Spring 2022
 */

#ifndef __WORD_H__
#define __WORD_H__

/**************** normalizeWord **************************/
/* Convert a word to lowercase 
 *`Return: 
 *   nothing if the word is null
 *   else, a word in canonical representation
 */
char *normalizeWord(char *word);

#endif
