/*
querier.h
Author: Godwin Kangor
Date: Nov  4, 2024
Description: h file for querier.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../common/index.h"
#include "../libcs50/counters.h"

/**************** parseArgs ****************/
/*
Caller provides:
    valid argc, valid argv[], pageDirectory and indexFilename

TODO:
    check that only 2 arguments are provided
    for pageDirectory, call pagedir_validatedir()
    for indexFile, check if the file is readable
    if any trouble is found, print an error to stderr and exit non-zero.
 */

static void parseArgs(const int argc, char *argv[],
                      char **pageDirectory, char **indexFile);

/**************** query ****************/
/*
Caller provides:
    valid pageDirectory and indexFile
TODO:
    get input from stdin (lineQuery)
        create an index and read the indexFile (indexer output)
        allocate memory for the word array and convert to string (toStringArray func)
        check for edge cases 'AND' and 'or' (correctSequence func)
            if array is not empty
                get the counters [docID, count] (processQuery func)
                print the matches (printMatches func)
        free lineQuery
        free wordArray
        delete the index

 */
static void query(char *pageDirectory, const char *indexFile);

/**************** toSringArray ****************/
/*
Converts a string of words into a string array

Caller provides:
    valid wordArray and lineQuery

TODO:
    loop through the word array
        skip over spaces at the start of the string
        loop through the characters to get the words in the string
            break if it gets to the end of the string
        replace space with end of string marker ['\0']
        normalized the word and add it to the array
        print the normalized word
        increment the word count
    return word count

RETURN:
    word count

 */
int toStringArray(char **wordArray, char *lineQuery);

/**************** correctSequence ****************/
/*
Converts a string of words into a string array

Caller provides:
    valid wordArray and word count
TODO:
    check for:
        case when OR or AND is the first
        case when there are two consecutive 'and's and/or 'or's
            - when when 'OR' is adjacent to 'AND' and vice versa
            - when two 'AND's are adjacent
            - when two 'OR's are adjacent
        case when OR or AND is the last
    return true (if passed all the cases above)


RETURN:
    true if word array is in the right sequence
    else print error message and return false

 */
bool correctSequence(char **wordArray, int numWords);

/**************** processQuery ****************/
/*
If
    - or is present, we merge count
    - and if present, intersect with the match
Caller provides:
    valid index, wordArray and numWords

TODO:
    check for:
        case when OR is present
            merge the count (mergeCount func)
            set shortcircuit to false
        if 'AND' is present, continue
        else:
            get the matches for this word - indexFind
            if matches is NULL
                set shortcircuit to True
                    if nothing was previously in the andSequence
                        delete the counter
            else: (match is not NULL)
                if nothing was previously in the andSequence
                    create and new counter for andSequence
                    join them (counterUnion func)
                else:
                    intersect counter with match (counterIntersect func)
    merge the count (mergeCount func)
    return orSequence

RETURN:
    orSequence counter
 */
counters_t *processQuery(index_t *index, char **wordArray, int numWords);

/**************** mergeCount ****************/
/*
Caller provides:
    valid andSequence and orSequence counters

TODO:
    handle error cases: check the andCounters is not NULL
    if orCounter is NUL create a new counter
    join the counters - counterUnion func
    delete andCounter

RETURN:
    void
 */
void mergeCount(counters_t **andSequence, counters_t **orSequence);

/**************** countersUnion ****************/
/*
Caller provides:
    valid counter1 and counter2

TODO:
    handle error cases
    iterate through the counters and set the counts
    uses helper function

RETURN:
    void
 */
void countersUnion(counters_t *counter1, counters_t *counter2);

/**************** countersUnion_helper ****************/
/*
Caller provides:
    valid arg, key and count

TODO:
    set the counts - get the counts from key


RETURN:
    void
 */
void countersUnion_helper(void *arg, const int key, const int count);

/**************** countersIntersect ****************/
/*
Caller provides:
    valid counter1 and counter2

TODO:
    handle error cases
    iterate the counter and set the counts with the min count
    uses helper function

RETURN:
    void
 */
void countersIntersect(counters_t *counter1, counters_t *counter2);

/**************** countersIntersect_helper ****************/
/*
Caller provides:
    valid arg, key and count

TODO:
    set the counts with the min count

RETURN:
    void
 */
void countersIntersect_helper(void *arg, const int key, const int count);

/**************** printMatches ****************/
/*
Caller provides:
    valid countersFound and pageDirectory

TODO:
    iterate through the counter and return counts (itemCount func)
    print the number of documents that matched
    iterate through the counters and get the largets count
    call print helper
    free largest and delete counter

RETURN:
    print the number of documents that matched
    else error message - No document found
 */
void printMatches(counters_t *countersFound, char *pageDirectory);

/**************** printMatches_Helper ****************/
/*
Caller provides:
    valid score, docID, and pageDirectory

TODO:
    create the path for pageDirectory (crawler output)
    read the file and get the URL (first line)
    if file in not NULL
        print message (score, docID and url found)
    close file
   
RETURN:
    print message (score, docID and url found)
 */
static void printMatches_Helper(const int score, const int docID, char *pageDirectory);