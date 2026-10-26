/*
 * word.c- 
 *
 * Provides functionality to normalize a word by converting it to lowercase.
 * Only words with a length of at least 3 characters are normalized.
 *
 * Author: Godwin Kangor
 * Date: October 28, 2024
 */
// word.c
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "word.h"

char *normalizeWord(char *word) {
    // Check for null or empty string
    if (word == NULL || strlen(word) < 3) {
        return NULL; // return NULL for invalid inputs
    }

    // Convert each character to lowercase
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = tolower(word[i]);
    }

    return word; // return the normalized word
}
