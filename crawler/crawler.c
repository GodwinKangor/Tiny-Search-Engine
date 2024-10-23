/*
 * crawlere.c - CS50 'crawler.c' module
 *
 * 
 *
 * Godwin Kangor, October 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webpage.h"
#include "bag.h"
#include "hashtable.h"
#include "pagedir.h"

static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);


// static function to parse arguments and ensure validity
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s seedURL pageDirectory maxDepth\n", argv[0]);
        exit(1);
    }

    *seedURL = normalizeURL(argv[1]);  // Duplicate the seed URL
    *pageDirectory = argv[2];    // Page directory does not need duplication
    *maxDepth = atoi(argv[3]);

    if (*maxDepth < 0 || *maxDepth > 10) {  // Arbitrary depth limit check
        fprintf(stderr, "Invalid maxDepth. Must be between 0 and 10.\n");
        exit(1);
    }
}

// function to handle the main crawl logic
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth) {
    // Initialize structures
    bag_t* pagesToCrawl = bag_new();
    hashtable_t* pagesSeen = hashtable_new(200);

    // Create the seed webpage
    webpage_t* seedPage = webpage_new(seedURL, 0, NULL);
    bag_insert(pagesToCrawl, seedPage);
    
    // Insert seedURL into the hashtable
    hashtable_insert(pagesSeen, seedURL, "");  // Ensure duplicate URL storage in hashtable

    // Process pages
    webpage_t* page;
    int docID = 1;  // Initialize docID for saving pages
    while ((page = bag_extract(pagesToCrawl)) != NULL) {
        if (webpage_fetch(page)) {
            printf("%d Fetched: %s\n", webpage_getDepth(page), webpage_getURL(page));

            // Save the page to the directory
            pagedir_save(page, pageDirectory, docID++);
            
            // Scan the page for more URLs if it's within the depth limit
            if (webpage_getDepth(page) < maxDepth) {
                pageScan(page, pagesToCrawl, pagesSeen);
            }
        }
        // Delete the page once it's fully processed
        webpage_delete(page);
    }

    // Clean up
    bag_delete(pagesToCrawl,webpage_delete);
    hashtable_delete(pagesSeen, NULL);  // Ensure hashtable_delete only frees allocated keys
}

// static function to scan the webpage and add internal URLs
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen) {
    char* nextURL;
    int pos = 0;  // Initialize position for webpage_getNextURL
    char* initialURL; 

    while ((initialURL = webpage_getNextURL(page, &pos)) != NULL) {
        printf("DEBUG: Next URL: %s\n", initialURL);
        nextURL = normalizeURL(initialURL);
        free(initialURL);
        if (nextURL != NULL) {
            printf("DEBUG: URL normalized: %s\n", nextURL);
            if (isInternalURL(nextURL)) {
                printf("DEBUG: URL is internal: %s\n", nextURL);

                printf("%d Found: %s\n", webpage_getDepth(page), nextURL);

                // Insert the normalized URL into the hashtable if not already seen
                if (hashtable_insert(pagesSeen, nextURL, "")) {
                    webpage_t* newPage = webpage_new(nextURL, webpage_getDepth(page) + 1, NULL);
                    bag_insert(pagesToCrawl, newPage);
                    printf("%d Added: %s\n", webpage_getDepth(page), nextURL);
                } else {
                    printf("%d IgnDupl: %s\n", webpage_getDepth(page), nextURL);
                    free(nextURL);  // Free the URL if it's a duplicate
                }
            } else {
                printf("DEBUG: URL is external: %s\n", nextURL);
                free(nextURL);  // Free external URLs immediately
            }
        } else {
            printf("DEBUG: Failed to normalize URL: %s\n", nextURL);
            
        }
    }
}

int main(int argc, char* argv[]) {
    char* seedURL = NULL;
    char* pageDirectory = NULL;
    int maxDepth = 0;

    // Parse the arguments
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);

    // Perform the crawl
    crawl(seedURL, pageDirectory, maxDepth);

    // Free allocated memory for the seed URL
    free(seedURL);

    return 0;
}
