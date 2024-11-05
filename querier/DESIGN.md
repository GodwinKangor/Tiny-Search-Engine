# CS50 Tiny Search Engine
## CS50 Fall 2024
## Design for Querier
## Author : Godwin Kipyatich Kangor
## 10/4/2024

## Introduction
Query processor takes crawler output and indexer output as inputs, accept user queries, and produce results based on the query criteria. The results include URLs, document IDs, and relevance scores (word counts) for documents that match the query.

## Program Structure

The program is structured into several functions and components, each responsible for specific tasks.

### Main Function
The main function serves as the entry point to the program. It performs the following tasks:
1. Parse command-line arguments, which should include the page directory (crawler output) and the index file (indexer output).
2. Call the `query` function to handle user queries.

### Parse Arguments
The `parseArgs` function is responsible for parsing and validating command-line arguments. It performs the following tasks:
1. Validates the page directory path to ensure it exists.
2. Checks if the index file is readable.

### Query Function
The `query` function is the core of the program and handles user queries. It performs the following tasks:
1. Continuously reads user queries from the standard input.
2. For each query, it does the following:
    - Creates an index based on the provided index file.
    - Converts the user query into an array of words and normalizes them.
    - Checks if the word sequence in the query is valid (i.e., no consecutive "and" or "or" and not starting or ending with "and" or "or").
    - Processes the query and retrieves the matching documents using the `processQuery` function.
    - Prints the matched documents, including their relevance scores, document IDs, and URLs.

### Process Query Function
The `processQuery` function processes a user query and returns a set of matching documents. It performs the following tasks:
1. Initializes two counters: `andSequence` and `orSequence` to handle "and" and "or" operations.
2. Iterates through the words in the user query:
    - If the word is "or," it merges the `andSequence` and `orSequence` counters.
    - If the word is "and," it continues to the next word.
    - For other words, it retrieves the document counters from the index and performs "and" or "or" operations accordingly.
3. Merges the `andSequence` and `orSequence` counters and returns the `orSequence` as the result.

### Helper Functions
- `toStringArray`: Converts a string of words into an array of normalized words.
- `correctSequence`: Validates the word sequence in the query to ensure it follows the correct format.
- `mergeCount`: Merges the `andSequence` and `orSequence` counters.
- `countersUnion`: Performs a union operation between two counters.
- `countersUnion_helper`: Helper function for counters union.
- `countersIntersect`: Performs an intersection operation between two counters.
- `countersIntersect_helper`: Helper function for counters intersection.
- `itemcount`: Counts the number of matched documents.
- `getLargest`: Finds the document with the highest relevance score.
- `printMatches_Helper`: Prints the matching document's information, including score, document ID, and URL.

## Data Structures
The program uses several data structures, including:
- `counters_t`: A data structure used to store document IDs and their corresponding word counts.
- `document_t`: A structure to represent a document with document ID and score.
- String arrays: Used to store and manipulate words in the user query.

## Error Handling
The program includes error handling for various scenarios, such as invalid command-line arguments, invalid page directory, unreadable index file, and incorrect query sequences.

## User Interaction
The program interacts with the user by continuously accepting and processing queries from the standard input. It provides feedback on the number of matching documents and displays relevant information about those documents.

## Conclusion
This C program serves as a query processor that takes input from a web crawler and indexer, processes user queries, and returns relevant documents based on the query criteria. It follows a modular and structured design to ensure the correctness and efficiency of the query processing.