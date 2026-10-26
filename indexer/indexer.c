/*
 * indexer.c - CS50 Tiny Search Engine Indexer
 *
 * This program indexes a set of webpages by reading each document,
 * extracting words, and storing the occurrences by document ID.
 * The index is then saved to an output file, which contains each word,
 * along with the document IDs and counts of occurrences in those documents.
 *
 * Author: Godwin Kangor
 * Date: October 28, 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagedir.h"
#include "index.h"
#include "word.h"

/**************** function declarations ****************/
static void indexBuild(const char* pageDirectory, index_t* index);
static void indexPage(webpage_t* page, const int docID, index_t* index);

/**************** main ****************/
/* Main function to create an index from a page directory
 * and save it to an output file.
 *
 * Caller provides:
 *   command-line arguments: the page directory and the index filename.
 * We return:
 *   0 if successful, 1 if incorrect usage, 2 if invalid directory,
 *   or 3 if failed to save the index.
 * We print:
 *   messages for successful directory validation and each saved word,
 *   or error messages as needed.
 */
int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s pageDirectory indexFilename\n", argv[0]);
        return 1;
    }

    const char* pageDirectory = argv[1];
    const char* indexFilename = argv[2];
    printf("Provided page directory: %s\n", pageDirectory);

    // Validate the page directory
    if (!pagedir_validate(pageDirectory)) {
        fprintf(stderr, "Invalid page directory\n");
        return 2;
    }

    // Create an index
    index_t* index = index_new(500);

    // Build the index from the page directory
    indexBuild(pageDirectory, index);

    // Save the index to the output file
    if (!index_save(index, indexFilename)) {
        fprintf(stderr, "Failed to save index to %s\n", indexFilename);
        index_delete(index);
        return 3;
    }

    index_delete(index);
    return 0;
}

/**************** indexBuild ****************/
/* Builds the index from a specified page directory.
 *
 * Caller provides:
 *   the page directory path and the index to populate.
 * We iterate through document files in the page directory,
 * loading each webpage and passing it to `indexPage` for word extraction.
 */
static void indexBuild(const char* pageDirectory, index_t* index) 
{
    int docID = 1;
    FILE *fp;
    char filePath[512];

    // Loop through files in pageDirectory using docID
    while (1) {
        snprintf(filePath, sizeof(filePath), "%s/%d", pageDirectory, docID);
        fp = fopen(filePath, "r");

        if (fp == NULL) {
            break;  // No more files to read, exit loop
        }

        // Use pagedir_load to load the webpage from the file
        webpage_t *webpage = pagedir_load(fp);
        fclose(fp); // Close the file after loading

        if (webpage != NULL) {
            indexPage(webpage, docID, index); // Index the webpage content
            webpage_delete(webpage);          // Clean up webpage memory
        } else {
            fprintf(stderr, "Warning: Failed to load page %d\n", docID);
        }

        docID++; // Move to the next document
    }
}

/**************** indexPage ****************/
/* Extracts words from a webpage and indexes them by document ID.
 *
 * Caller provides:
 *   a webpage, the document ID, and the index to populate.
 * For each word found in the webpage:
 *   - Words of length less than 3 are skipped.
 *   - Words are normalized to lowercase and stored in the index
 *     along with their document ID and count.
 */
static void indexPage(webpage_t* page, const int docID, index_t* index) 
{
    int pos = 0;
    char* word;
    while ((word = webpage_getNextWord(page, &pos)) != NULL) {
        if (strlen(word) >= 3) {
            normalizeWord(word);
            index_insert(index, word, docID);
        }
        free(word);
    }
}