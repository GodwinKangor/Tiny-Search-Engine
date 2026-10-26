/*
 * index.h - header file for CS50 'index' module
 * 
 * The 'index' module provides a data structure for storing words and their occurrences 
 * in documents. It uses a hashtable to map words (as keys) to counters that track 
 * the document ID (docID) and the number of times the word appears in that document.
 *
 * Author: Godwin Kangor
 * Date: October 28, 2024
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include "../libcs50/hashtable.h"
#include "counters.h"

/**************** global types ****************/
typedef hashtable_t index_t;  // index is a type of hashtable where keys are words, items are counters

/**************** functions ****************/

/**************** index_new ****************/
/* Create a new (empty) index with a specified number of slots in the hashtable.
 *
 * Caller provides:
 *   the number of slots (num_slots) for the hashtable.
 * We return:
 *   pointer to a new index, or NULL if error.
 * We guarantee:
 *   The index is initialized as an empty hashtable.
 * Caller is responsible for:
 *   later calling index_delete.
 */
index_t* index_new(const int num_slots);

/**************** index_insert ****************/
/* Insert a word and document ID (docID) into the index, updating the count if it already exists.
 *
 * Caller provides:
 *   a valid index pointer, a valid word (string), and a docID (int).
 * We guarantee:
 *   the index will contain the word mapped to a counter structure.
 *   if the word already exists in the index, its count for the docID will be incremented.
 * Caller is responsible for:
 *   ensuring the index, word, and docID are valid.
 */
void index_insert(index_t* index, const char* word, int docID);

/**************** index_save ****************/
/* Save the index to a file.
 *
 * Caller provides:
 *   a valid index pointer and a filename (string).
 * We return:
 *   true if the index was successfully saved, false otherwise.
 * We guarantee:
 *   if the file is accessible, it will contain the index data.
 * Caller is responsible for:
 *   managing the output file and handling the return status.
 */
bool index_save(index_t* index, const char* filename);

/**************** index_load ****************/
/* Load an index from a file.
 *
 * Caller provides:
 *   a filename (string) from which to load the index.
 * We return:
 *   pointer to a new index containing the data from the file, or NULL if error.
 * Caller is responsible for:
 *   handling any loading errors and later calling index_delete.
 */
index_t* index_load(const char* filename);

/**************** index_delete ****************/
/* Delete the entire index.
 *
 * Caller provides:
 *   a valid index pointer.
 * We guarantee:
 *   all memory associated with the index is freed.
 * Notes:
 *   the function is safe to call with a NULL index.
 */
void index_delete(index_t* index);

// index_find
counters_t *index_find(index_t *index, char *word);

#endif // __INDEX_H