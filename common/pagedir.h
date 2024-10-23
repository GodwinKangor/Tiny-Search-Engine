/* 
 * pagedir.h
 * Godwin , 2024FA
 */
#ifndef __PAGEDIR_H__
#define __PAGEDIR_H__

#include <stdbool.h>            // Ensure bool is defined
#include "../libcs50/webpage.h"  // Correctly include webpage.h

/* Initializes a page directory by creating a .crawler file in the directory.
 * Returns true if successful, false otherwise.
 */
bool pagedir_init(const char* pageDirectory);

/* Saves the given webpage to a file in the specified page directory.
 * The file is named as the next document ID (docID), and contains the URL,
 * depth, and HTML content of the webpage.
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

#endif // __PAGEDIR_H__
