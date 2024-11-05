/*
 *indextest.c
 *
 * Author: Godwin Kangor
 * Date: October 28, 2024
 */
#include "index.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../common/index.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s oldIndexFilename newIndexFilename\n", argv[0]);
        return 1;
    }

    const char* oldIndexFilename = argv[1];
    const char* newIndexFilename = argv[2];

    index_t* index = index_load(oldIndexFilename);
    if (index == NULL) {
        fprintf(stderr, "Failed to load index from %s\n", oldIndexFilename);
        return 2;
    }

    if (!index_save(index, newIndexFilename)) {
        fprintf(stderr, "Failed to save index to %s\n", newIndexFilename);
        index_delete(index);
        return 3;
    }

    index_delete(index);
    return 0;
}