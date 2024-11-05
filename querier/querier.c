/*
querier.c
Author: Godwin Kangor
Date: Nov 4 ,2024
Description:  C program that takes crawler output and indexer output,
    accepts user queries, and produces results, including URLs, document IDs,
    and relevance scores (word counts) based on the query's criteria.
*/

#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "querier.h"
#include "../common/pagedir.h"
#include "../libcs50/file.h"
#include "../libcs50/mem.h"
#include "../common/word.h"
#include <strings.h>  // For strcasecmp


//
struct twoCounters
{
    counters_t *result;
    counters_t *other;
};

// document contains docID and score
typedef struct document
{
    int docID;
    int score;
} document_t;

// defined functions
static void parseArgs(const int argc, char *argv[],
                      char **pageDirectory, char **indexFile);
static void query(char *pageDirectory, const char *indexFile);
int toStringArray(char **wordArray, char *lineQuery);
bool correctSequence(char **wordArray, int numWords);
counters_t *processQuery(index_t *index, char **wordArray, int numWords);
void printMatches(counters_t *countersFound, char *pageDirectory);
void mergeCount(counters_t **andSequence, counters_t **orSequence);
void countersUnion(counters_t *counter1, counters_t *counter2);
void countersUnion_helper(void *arg, const int key, const int count);
void countersIntersect(counters_t *counter1, counters_t *counter2);
void countersIntersect_helper(void *arg, const int key, const int count);
static void itemcount(void *arg, const int key, const int count);
static void getLargest(void *arg, const int docID, const int score);
static void printMatches_Helper(const int score, const int docID, char *pageDirectory);

// main function
int main(const int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s: [pageDirectory] [indexFile]\n", argv[0]);
        exit(1);
    }

    // initiate pageDirectory and indexFile
    char *pageDirectory = NULL;
    char *indexFile = NULL;

    // validate the arguments then call query
    parseArgs(argc, argv, &pageDirectory, &indexFile);
    query(pageDirectory, indexFile);
}

// parseArgs function----> see querier.h for more info
static void parseArgs(const int argc, char *argv[],
                      char **pageDirectory, char **indexFile)
{
    // Allocate values for pageDirectory (argv[1]) and indexFile (argv[2])
    *pageDirectory = strdup(argv[1]);
    *indexFile = strdup(argv[2]);

    // Validate pageDirectory
    if (!pagedir_validate(*pageDirectory))
    {
        fprintf(stderr, "Error: Invalid pageDirectory path\n");
        exit(2);
    }

    // Validate that indexFile is readable
    FILE *fp = fopen(*indexFile, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: %s is not readable\n", *indexFile);
        exit(3);
    }
    fclose(fp);// Only close if fopen succeeded

}


// query
static void query(char *pageDirectory, const char *indexFile)
{
    char *lineQuery; // input from stdin
    printf("Query: ");
    while ((lineQuery = file_readLine(stdin)) != NULL)
    {
        // create an index and read the indexFile (indexer output)
        index_t *index = index_load(indexFile);

        // allocate memory for the word array and convert to string
        char **wordArray = mem_malloc(strlen(lineQuery) * sizeof(char *));
        int numWords = toStringArray(wordArray, lineQuery);

        // Check for edge cases ('AND' and 'or')
        if (correctSequence(wordArray, numWords))
        {
            // if array is not empty
            if (numWords > 0)
            {
                // get the counters [docID, count] and print the matches
                counters_t *countersFound = processQuery(index, wordArray, numWords);
                printMatches(countersFound, pageDirectory);
            }
        }
        mem_free(lineQuery);
        mem_free(wordArray);
        index_delete(index);
        printf("\n\nQuery: ");
    }
    printf("\n");
}

// processQuery----> see querier.h for more information
counters_t *processQuery(index_t *index, char **wordArray, int numWords)
{
    counters_t *andSequence = NULL;
    counters_t *orSequence = NULL;
    bool shortCircuit = false;

    for (int i = 0; i < numWords; i++)
    {
        // case when OR is present, merge the count
        if (strcmp(wordArray[i], "or") == 0)
        {
            mergeCount(&andSequence, &orSequence);
            shortCircuit = false;
            continue;
        }
        if (shortCircuit)
        {
            // skip over words until or
            continue;
        }
        // skip over and to the next word
        if (strcmp(wordArray[i], "and") == 0)
        {
            continue;
        }
        else
        {
            // get pages for this word
            counters_t *match = index_find(index, wordArray[i]);
            // if query is and and match of previous word was not found, set everything to null
            if (match == NULL)
            {
                shortCircuit = true;

                // if nothing was previously in the andSequence delete the counter
                if (andSequence != NULL)
                {
                    counters_delete(andSequence);
                    andSequence = NULL;
                }
            }
            else
            {
                // if nothing was previously in the andsequence
                if (andSequence == NULL)
                {
                    // create and new counter for andSequence and join them
                    andSequence = counters_new();
                    countersUnion(andSequence, match);
                }
                else
                {
                    countersIntersect(andSequence, match);
                }
            }
        }
    }
    mergeCount(&andSequence, &orSequence);
    return orSequence;
}

// converts a string of words into a string array
int toStringArray(char **wordArray, char *lineQuery)
{
    int numWords = 0;               // position in the array
    int length = strlen(lineQuery); // length of the array

    for (int i = 0; i <= length; i++)
    {
        // skip over spaces at the start of the string
        if (isspace(lineQuery[i]))
        {
            i++;
            continue;
        }

        // loop through the characters to get the words in the string
        if (isalpha(lineQuery[i]))
        {
            char *word = &lineQuery[i];
            while (!isspace(lineQuery[i]) && lineQuery[i] != '\0')
            {
                i++;
            }

            // replace space with end of string marker
            lineQuery[i] = '\0';

            // normalize the word and add it to the array
            char *normalizedWord = normalizeWord(word);

            // Check for valid operators "and" or "or" even if normalization fails
            if (normalizedWord == NULL && !(strcasecmp(word, "and") == 0 || strcasecmp(word, "or") == 0)) {
                fprintf(stderr, "Invalid word: %s\n", word);
                return -1;
            }

            // Use the normalized word if valid, otherwise add "and" or "or" directly
            wordArray[numWords] = normalizedWord ? normalizedWord : strdup(word);
            printf("%s ", wordArray[numWords]); // print the normalized word or operator

            numWords++; // increment the word count
        }
        else {
            fprintf(stderr, "Invalid character provided\n");
            return -1;
        }
    }
    return numWords; // return word count
}


// correctSequence
bool correctSequence(char **wordArray, int numWords)
{
    for (int i = 0; i < numWords; i++)
    {
        // case when OR or AND is the first
        if ((i == 0 && strcmp(wordArray[i], "or") == 0) || (i == 0 && strcmp(wordArray[i], "and") == 0))
        {
            fprintf(stderr, "Invalid input: '%s' cannot be the first\n", wordArray[i]);
            return false;
        }

        // case when there are two consecutive 'and's and/or 'or's
        if (i + 1 < (numWords - 1))
        {
            // case when 'OR' is adjacent to 'AND' / 'AND' is adjacent to 'OR'
            if (((strcmp(wordArray[i], "or") == 0) && (strcmp(wordArray[i + 1], "and") == 0)) || ((strcmp(wordArray[i], "and") == 0) && (strcmp(wordArray[i + 1], "or") == 0)))
            {
                fprintf(stderr, "Invalid input: '%s' and '%s' cannot be adjacent\n", wordArray[i], wordArray[i + 1]);
                return false;
            }
            // case when two 'AND's are adjacent
            if (strcmp(wordArray[i], "and") == 0 && strcmp(wordArray[i + 1], "and") == 0)
            {
                fprintf(stderr, "Invalid input: two '%s'cannot be adjacent\n", wordArray[i]);
                return false;
            }

            // case when two 'OR's are adjacent
            if (strcmp(wordArray[i], "or") == 0 && strcmp(wordArray[i + 1], "or") == 0)
            {
                fprintf(stderr, "Invalid input: two '%s' cannot be adjacent\n", wordArray[i]);
                return false;
            }
        }
        // case when OR or AND is the last
        if (i == numWords - 1)
        {
            if (strcmp(wordArray[i], "or") == 0 || strcmp(wordArray[i], "and") == 0)
            {
                fprintf(stderr, "Invalid input: '%s' cannot be the last\n", wordArray[i]);
                return false;
            }
        }
    }
    return true;
}

// merges count
void mergeCount(counters_t **andSequence, counters_t **orSequence)
{
    // handle error cases
    if (*andSequence != NULL)
    {
        // if orCounter is NUL create a new counter
        if (*orSequence == NULL)
        {
            *orSequence = counters_new();
        }
        // join the counters and delete andCounter
        countersUnion(*orSequence, *andSequence);
        counters_delete(*andSequence);
        *andSequence = NULL;
    }
}

// counter
void countersUnion(counters_t *counter1, counters_t *counter2)
{
    // handle error cases
    mem_assert(counter1, "counters 1 invalid");
    mem_assert(counter2, "counters 2 invalid");

    // iterate through the counters and set the counts
    counters_iterate(counter2, counter1, countersUnion_helper);
}

// Counter union Helper
void countersUnion_helper(void *arg, const int key, const int count)
{
    // set the counts - get the counts from key
    counters_set(arg, key, count + counters_get(arg, key));
}

// countersIntersect
void countersIntersect(counters_t *counter1, counters_t *counter2)
{
    // handle error cases
    mem_assert(counter1, "counters 1 invalid");
    mem_assert(counter2, "counters 2 invalid");

    // create two counters
    struct twoCounters args = {counter1, counter2};

    // iterate the counter and set the counts with the min count
    counters_iterate(counter1, &args, countersIntersect_helper);
}

static inline int min(const int num1, const int num2)
{
    return (num1 < num2 ? num1 : num2);
}

// counter intersect helper
void countersIntersect_helper(void *arg, const int key, const int count)
{
    struct twoCounters *two = arg;

    // set the counts with the min count
    counters_set(two->result, key, min(count, counters_get(two->other, key)));
}

// print the results
void printMatches(counters_t *countersFound, char *pageDirectory)
{
    if (countersFound != NULL)
    {
        // iterate through the counter and return counts
        int matches = 0;
        counters_iterate(countersFound, &matches, itemcount);

        // print the number of documents that matched
        printf("\nMatches %d documents (ranked): \n", matches);

        // create largest data structure and set docID and score to 0
        document_t *largest = mem_malloc(sizeof(document_t));
        largest->docID = 0;
        largest->score = 0;

        for (int i = 0; i < matches; i++)
        {
            // iterate through the counters and get the largets count
            counters_iterate(countersFound, largest, getLargest);

            // call print helper
            printMatches_Helper(largest->score, largest->docID, pageDirectory);
            counters_set(countersFound, largest->docID, 0);
            largest->score = 0;
        }

        mem_free(largest);
        counters_delete(countersFound);
    }
    else
    {
        printf("\nNo documents match.\n");
    }
}

// itemCount
static void itemcount(void *arg, const int key, const int count)
{

    int *matches = arg;
    if (matches != NULL && count > 0)
    {
        (*matches)++;
    }
}

// get largest
static void getLargest(void *arg, const int docID, const int score)
{
    document_t *node = arg;
    if (node->score <= score)
    {
        node->docID = docID;
        node->score = score;
    }
}

// printMatches Helper
static void printMatches_Helper(const int score, const int docID, char *pageDirectory)
{
    // create the path for pageDirectory (crawler output)
    FILE *fp;
    char *filename = mem_malloc(strlen(pageDirectory) + strlen("/") + sizeof(int) + 1);
    sprintf(filename, "%s/%d", pageDirectory, docID);

    // read the file and get the URL (first line)
    fp = fopen(filename, "r");
    if (fp != NULL)
    {
        char *URL = file_readLine(fp); // URL is the first line

        // print message (score, docID and url found)
        printf("score:\t%d doc\t%d: %s\n", score, docID, URL);
        mem_free(URL);
        fclose(fp);
    }
    mem_free(filename);
}