# CS50 Tiny Search Engine
## CS50 Fall 2024
## Readme Implementation
## Author: Godwin Kipyatich Kangor
## Date: Nov 04, 2024

### Querier

# CS50 TSE Querier
## Implementation Specification

In this document, we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions. The knowledge unit noted that an [implementation spec](https://github.com/cs50fall2023/home/blob/main/knowledge/units/design.md#implementation-spec) may include many topics; not all are relevant to the TSE or the querier. Here we focus on the core subset:

- Data structures
- Control flow: pseudocode for overall flow, and for each function
- Detailed function prototypes and their parameters
- Error handling and recovery
- Testing plan

## Control Flow

The querier is implemented in one file `querier.c`, with 11 functions and 2 helper functions.

### `main`

The `main` function calls `parseArgs` and `query`, then exits with status zero.

### `parseArgs`

Given arguments from the command line, this function extracts them into parameters and returns only if successful.
- Checks that exactly two arguments are provided.
- For `pageDirectory`, calls `pagedir_validatedir()`.
- Ensures `indexFilename` is writable.
- Prints an error to `stderr` and exits non-zero if any issues are found.

### `query`

Pseudocode:

    get input from stdin (lineQuery)
        create an index and read the indexFile (querier output)
        allocate memory for the word array and convert to string (toStringArray func)
        check for edge cases 'AND' and 'or' (correctSequence func)
            if array is not empty
                get the counters [docID, count] (processQuery func)
                print the matches (printMatches func)
        free lineQuery
        free wordArray
        delete the index

### `toStringArray`

Pseudocode:

    loop through the word array
        skip over spaces at the start of the string
        loop through the characters to get the words in the string
            break if it gets to the end of the string
        replace space with end of string marker ['\0']
        normalized the word and add it to the array
        print the normalized word
        increment the word count
    return word count

### `correctSequence`

Pseudocode:

    check for:
        case when OR or AND is the first
        case when there are two consecutive 'and's and/or 'or's
            - when 'OR' is adjacent to 'AND' and vice versa
            - when two 'AND's are adjacent
            - when two 'OR's are adjacent
        case when OR or AND is the last
    return true (if passed all the cases above)

### `processQuery`

Pseudocode:

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
                    create a new counter for andSequence
                    join them (counterUnion func)
                else:
                    intersect counter with match (counterIntersect func)
    merge the count (mergeCount func)
    return orSequence

### `mergeCount`

Pseudocode:

    handle error cases: check the andCounters is not NULL
    if orCounter is NULL, create a new counter
    join the counters - counterUnion func
    delete andCounter

### `countersUnion`

Pseudocode:

    handle error cases
    iterate through the counters and set the counts
    uses helper function

### `countersIntersect`

Pseudocode:

    handle error cases
    iterate the counter and set the counts with the min count
    uses helper function

### `printMatches`

Pseudocode:

    iterate through the counter and return counts (itemCount func)
    print the number of documents that matched
    iterate through the counters and get the largest count
    call print helper
    free largest and delete counter

### `printMatches_Helper`

Pseudocode:

    create the path for pageDirectory (crawler output)
    read the file and get the URL (first line)
    if file is not NULL
        print message (score, docID, and URL found)
    close file

## Function Prototypes

Detailed descriptions of each function's interface are provided as comments before each function's implementation in `querier.c` and are not repeated here.

```c
int main(const int argc, char *argv[]);
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
static void itemcount(void *arg, const int key, const int count);
static void getLargest(void *arg, const int docID, const int score);
static void printMatches_Helper(const int score, const int docID, char *pageDirectory);

```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.


### Integration/system testing

We write a script `testing.sh` that invokes the querier several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.

Run that script with `bash -v testing.sh` so the output of querier is intermixed with the commands used to invoke the querier.
Verify correct behavior by studying the output, and by sampling the files created in the respective pageDirectories.