/*
 * pagedir.c - CS50 'pagedir.c' module
 *
 * 
 *
 * Godwin Kangor, October 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagedir.h"
#include "../libcs50/webpage.h"   // Ensure we include webpage.h correctly

/* Initializes the page directory by creating a .crawler file in it */
bool pagedir_init(const char* pageDirectory) {
    char crawlerFilePath[512];
    snprintf(crawlerFilePath, sizeof(crawlerFilePath), "%s/.crawler", pageDirectory);

    FILE* fp = fopen(crawlerFilePath, "w");
    if (fp == NULL) {
        return false;
    }

    fclose(fp);
    return true;
}

/* Saves a webpage to the page directory with the given docID */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) {
    char filePath[512];
    snprintf(filePath, sizeof(filePath), "%s/%d", pageDirectory, docID);

    FILE* fp = fopen(filePath, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot save page to %s\n", filePath);
        return;
    }

    fprintf(fp, "%s\n", webpage_getURL(page));          // URL
    fprintf(fp, "%d\n", webpage_getDepth(page));        // Depth
    fprintf(fp, "%s\n", webpage_getHTML(page));         // HTML Content

    fclose(fp);
}
