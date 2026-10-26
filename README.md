
# CS50 TSE
## Godwin Kangor  (GodwinKangor)


# Tiny Search Engine (TSE)

## 📖 Overview
The **Tiny Search Engine (TSE)** is a simplified web search engine implemented in **C** as part of CS50 coursework.  
It demonstrates the fundamental components of a search engine:

1. **Crawler** – fetches pages starting from a seed URL, explores links up to a given depth, and saves them to disk.  
2. **Indexer** – parses saved pages, tokenizes text, and builds an **inverted index** mapping words → document IDs → counts.  
3. **Querier** – reads user queries (with `AND`/`OR` logic), retrieves matching documents from the index, and ranks them by relevance.
```
## 🛠️ Project Structure

```markdown
tiny-search-engine/
│
├── crawler/ # Crawls web pages starting from a seed URL
│ ├── crawler.c
│ └── ...
│
├── indexer/ # Builds the inverted index from crawled pages
│ ├── indexer.c
│ └── ...
│
├── querier/ # Processes user queries against the index
│ ├── querier.c
│ └── ...
│
├── common/ # Shared modules (e.g., bag, set, hashtable, counters)
│ ├── pagedir.c
│ ├── word.c
│ └── ...
│
└── README.md
```

### 2. Indexer

* **Input**: `pageDirectory`, `indexFilename`
* Reads crawled pages, tokenizes words, and builds an inverted index.
* Writes the index to a file (`indexFilename`).

**Example usage:**

```bash
./indexer data/ index.out
```

### 3. Querier

* **Input**: `indexFilename`
* Accepts user queries from stdin.
* Supports `AND` / `OR` operators, ranks results by word frequency, and prints matched document IDs with scores.

**Example usage:**

```bash
./querier data/ index.out
```

---

## Data Structures

* **Bag** – collection of items (used in crawler to store URLs).
* **Set** – key/value storage (used for visited URLs).
* **Counters** – maps integers → counts (used in indexer for word frequencies).
* **Hashtable** – fast key/value store (used to map words to counters).

---

## Features

* Depth-limited crawling
* Duplicate URL avoidance
* Word normalization (lowercasing, stripping punctuation)
* Boolean query logic (`AND`, `OR`)
* Ranked results by word occurrence

---

## Compilation

To build all modules:

```bash
make
```

To clean up binaries:

```bash
make clean
```

---

## Testing

1. Run the crawler to fetch test pages
2. Build the index using the indexer
3. Query the index using the querier

Provided test cases (like `letters` or `toscrape`) verify correctness.

---

## Lessons Learned

* Memory management in C (with `valgrind` debugging)
* Implementation of fundamental data structures (bag, set, counters, hashtable)
* Parsing and tokenization of HTML
* Query processing and ranking logic similar to real search engines

---

## Author

Developed by **Godwin Kangor** as part of **CS50 Tiny Search Engine Project**.




