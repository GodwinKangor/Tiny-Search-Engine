# Common Module

## Overview
The `common` directory contains utility modules that provide shared functionality for various parts of the Tiny Search Engine (TSE) project. Specifically, it includes the `pagedir` module, which handles directory operations for saving and loading webpage data.

The `common` module is used by the `crawler` to save fetched pages to disk, and by later components (such as the indexer and querier) to retrieve and process those pages.

## Contents

- `pagedir.c` and `pagedir.h`: Implements functions for saving webpages to disk and validating directories. These functions help manage webpage files during the crawling and indexing process.

## Files
- **pagedir.h/pagedir.c**: This module provides the following key functions:
  - `pagedir_init()`: Initializes the page directory by ensuring it is writable and creating a `.crawler` file to mark it as a valid directory.
  - `pagedir_save()`: Saves a fetched webpage's HTML to a specified directory, assigning each page a unique document ID.
  
## Compilation
To compile the `common` module, use the `make` command:

```bash
make
