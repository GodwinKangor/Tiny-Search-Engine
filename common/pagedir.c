/*
 * pagedir.c - CS50 'pagedir.c' module
 *
 * 
 *
 * Godwin Kangor, October 2024
 */
#define _GNU_SOURCE  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagedir.h"
#include "../libcs50/webpage.h"   // Ensure we include webpage.h correctly
#include "webpage.h"
#include "mem.h"
#include "file.h"

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

char* create_pathname(const char* filename, const char* pageDirectory) {
    // Allocate enough space for the full pathname
    // strlen + 1 (for null terminator) + 1 (for the possible '/' separator)
    int total_length = strlen(pageDirectory) + strlen(filename) + 2; // +2 for '/' and '\0'
    char* pathname = malloc(total_length);

    // Check if malloc succeeded
    if (pathname == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Copy the directory path to the pathname
   
    sprintf(pathname, "%s/%s", pageDirectory, filename);
    // // Ensure the directory ends with a '/'
    // if (pageDirectory[strlen(pageDirectory) - 1] != '/') {
    //     strcat(pathname, "/");
    // }

    // // Concatenate the filename to the pathname
    // strcat(pathname, filename);

   

    return pathname;
}


bool pagedir_validate(const char* pageDirectory) {
    char* file = create_pathname(".crawler",pageDirectory);
    printf("Checking for file: %s\n", file);
    if (file == NULL) {
        fprintf(stderr, "Failed to create pathname\n");
        return false;
    }

    printf("Checking for file: %s\n", file); // Debug output

    FILE* fp = fopen(file, "r");  // Open in read mode to check existence
    free(file);  // Free after use

    if (fp == NULL) {
        fprintf(stderr, "File does not exist or is inaccessible\n");
        return false;
    }

    printf("valid directory"); 
    fclose(fp);
    return true;
}


// Loads a page from a file
webpage_t *pagedir_load(FILE *fp) {
    // Read the URL
    char *url = file_readLine(fp);
    if (url == NULL) {
        fprintf(stderr, "Error: Failed to read URL from file\n");
        return NULL;
    }

    // Read the depth
    char *depthStr = file_readLine(fp);
    if (depthStr == NULL) {
        fprintf(stderr, "Error: Failed to read depth from file\n");
        mem_free(url);
        return NULL;
    }
    int depth = atoi(depthStr);
    mem_free(depthStr);

    // Read the HTML content
    char *html = file_readFile(fp);
    if (html == NULL) {
        fprintf(stderr, "Error: Failed to read HTML from file\n");
        mem_free(url);
        return NULL;
    }

    // Construct the webpage object from the stored data
    webpage_t *page = webpage_new(url, depth, html);
    if (page == NULL) {
        fprintf(stderr, "Error: Failed to create webpage\n");
        mem_free(url);
        mem_free(html);
        return NULL;
    }

    return page;
}