#!/bin/bash
# testing.sh
# Author: Godwin Kangor
# Date: Nov 04, 2024
# Description: Bash script to test querier.c 

# Paths to directories and files
PAGE_DIRECTORY="$HOME/cs50-dev/tse-GodwinKangor/data/letters-3"
INDEX_FILE="$HOME/cs50-dev/tse-GodwinKangor/indexer/index.out"
INVALID_DIRECTORY="$HOME/cs50-dev/tse-GodwinKangor/data/invalid-directory"
INVALID_INDEX_FILE="$HOME/cs50-dev/tse-GodwinKangor/indexer/invalid_index.out"

# Header
echo -e "\nRunning tests for querier.c\n"
echo -e "===============================================\n"

# Testing with an invalid pageDirectory
echo -e "\nTESTING: Invalid pageDirectory path"
echo "Query: \"page\""
echo "page" | ./querier "$INVALID_DIRECTORY" "$INDEX_FILE"
echo -e "-----------------------------------------------"

# Testing with an invalid indexFile
echo -e "\nTESTING: Invalid indexFile path"
echo "Query: \"page\""
echo "page" | ./querier "$PAGE_DIRECTORY" "$INVALID_INDEX_FILE"
echo -e "-----------------------------------------------"

# Testing invalid use of AND/OR
echo -e "\nTESTING: 'AND' at the beginning of the query"
echo "Query: \"and\""
echo "and" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"
echo -e "-----------------------------------------------"

echo -e "\nTESTING: 'OR' at the beginning of the query"
echo "Query: \"or\""
echo "or" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"
echo -e "-----------------------------------------------"

echo -e "\nTESTING: 'AND' at the end of the query"
echo "Query: \"home And\""
echo "home And" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"
echo -e "-----------------------------------------------"

echo -e "\nTESTING: 'OR' at the end of the query"
echo "Query: \"home or\""
echo "home or" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"
echo -e "-----------------------------------------------"

echo -e "\nTESTING: 'AND' adjacent to 'OR'"
echo "Query: \"home and Or for\""
echo "home and Or for" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"
echo -e "-----------------------------------------------"

# Valid queries with expected results
echo -e "\nRunning valid queries\n"

echo -e "\nVALID QUERY: Simple single word query"
echo "Query: \"page\""
echo "page" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"
echo -e "-----------------------------------------------"

echo -e "\nVALID QUERY: Query with 'OR' operator"
echo "Query: \"page or huffman\""
echo "page or huffman" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"
echo -e "-----------------------------------------------"

echo -e "\nVALID QUERY: Query with 'AND' operator"
echo "Query: \"biology and fast\""
echo "biology and fast" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"
echo -e "-----------------------------------------------"

echo -e "\nVALID QUERY: Combined 'AND' and 'OR' operators"
echo "Query: \"biology or fast\""
echo "biology or fast" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"
echo -e "-----------------------------------------------"

echo -e "\nVALID QUERY: Complex query with 'AND' and 'OR'"
echo "Query: \"biology and fast or search\""
echo "biology and fast or search" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"
echo -e "-----------------------------------------------"

# Testing with invalid characters
echo -e "\nTesting queries with invalid characters\n"

echo -e "\nINVALID CHARACTER TEST: Query with special characters"
echo "Query: \"home ``\""
echo "home ``" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"
echo -e "-----------------------------------------------"

echo -e "\nINVALID CHARACTER TEST: Query starting with numbers/special chars"
echo "Query: \"``3\""
echo "``3" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"
echo -e "-----------------------------------------------"

# Summary
echo -e "\nAll tests completed.\n"
