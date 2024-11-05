/*
 * index.c
 * 
 * The 'index' module provides a data structure for storing words and their occurrences 
 * in documents. It uses a hashtable to map words (as keys) to counters that track 
 * the document ID (docID) and the number of times the word appears in that document.
 *
 * Author: Godwin Kangor
 * Date: October 28, 2024
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "counters.h"
#include "../libcs50/hashtable.h"


static void counters_free(void* item) 
{
    counters_delete(item);
}

static void save_counter(void* fp, const int key, const int count) 
{
    FILE* file = (FILE*)fp;
    fprintf(file, " %d %d", key, count);  // Print docID and count for each word
    // Debug output
    printf("Saved docID: %d with count: %d\n", key, count);
}

static void save_to_file(void* fp, const char* word, void* item) 
{
    FILE* file = (FILE*)fp;
    counters_t* counters = (counters_t*)item;

    fprintf(file, "%s", word);  // Print the word itself
    counters_iterate(counters, file, save_counter);  // Print each docID and count for the word
    fprintf(file, "\n");  // Newline after each word entry

    // Optional debug output
    printf("Saved word: %s\n", word);
}




// Index functions

index_t* index_new(const int num_slots) 
{
    return hashtable_new(num_slots);
}

void index_insert(index_t* index, const char* word, int docID)
{
    counters_t* counters = hashtable_find(index, word);  // Find existing counters for the word
    if (counters == NULL) {
        counters = counters_new();  // Create new counters if none exist
        hashtable_insert(index, word, counters);  // Insert word and counters into the hashtable
    }
    counters_add(counters, docID);  // Increment counter for this document ID
}

//see index.h for more info
bool index_save(index_t* index, const char* filename) 
{
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) return false;
    hashtable_iterate(index, fp, save_to_file);
    fclose(fp);
    return true;
}


index_t* index_load(const char* filename) 
{
    index_t* index = index_new(500);
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) return NULL;
    char word[100];
    int docID, count;
    while (fscanf(fp, "%s", word) != EOF) {
        counters_t* counters = counters_new();
        while (fscanf(fp, "%d %d", &docID, &count) == 2) {
            counters_set(counters, docID, count);
        }
        hashtable_insert(index, word, counters);
    }
    fclose(fp);
    return index;
}

void index_delete(index_t* index) 
{
    hashtable_delete(index, counters_free);
}

// index_find
counters_t *index_find(index_t *index, char *word)
{
    // directly find the counter in the hashtable
    counters_t *item = hashtable_find(index, word);
    return item;
}
