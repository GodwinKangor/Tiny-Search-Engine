/*
 * word.h - header file for CS50 'word' module
 *
 * Provides functionality to normalize a word by converting it to lowercase.
 * Only words with a length of at least 3 characters are normalized.
 *
 * Author: Godwin Kangor
 * Date: October 28, 2024
 */

#ifndef __WORD_H
#define __WORD_H

#include <stdbool.h>

/**************** functions ****************/

/**************** normalizeWord ****************/
/* Converts a word to lowercase in-place, modifying the original string.
 *
 * Caller provides:
 *   a valid string pointer `word` to normalize.
 * We return:
 *   true if the word was normalized (modified), false otherwise.
 * Notes:
 *   Only words of length 3 or greater are considered for normalization.
 */
char  *normalizeWord(char *word);

#endif // __WORD_H