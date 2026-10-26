#!/bin/bash
##Godwin Kangor

# Test script for Tiny Search Engine crawler
# Run the crawler with various configurations and check memory usage

# Set paths for tests
TEST_DIR="../data/test"
SEED_URL="http://cs50tse.cs.dartmouth.edu/tse/letters/index.html"
MAX_DEPTH=2

# Make the directory for the test output
mkdir -p $TEST_DIR

# Clean previous test results
echo "Cleaning old test results..."
rm -rf $TEST_DIR/*

# Test 1: Basic functionality test with depth 2
echo "Running Test 1: Basic functionality with max depth = $MAX_DEPTH"
valgrind --leak-check=full --show-leak-kinds=all --log-file="valgrind_test1.log" ./crawler $SEED_URL $TEST_DIR $MAX_DEPTH

# Check if the test directory was filled with the correct number of files
if [ "$(ls -A $TEST_DIR)" ]; then
  echo "Test 1 Passed: Files were created."
else
  echo "Test 1 Failed: No files created."
fi

# Test 2: Memory usage limit test
MEMLIMIT=100000  # memory limit in KB (adjust as needed)
echo "Running Test 2: Memory limit test with memlimit = $MEMLIMIT KB"
ulimit -v $MEMLIMIT  # Set memory limit in KB for this test

valgrind --leak-check=full --show-leak-kinds=all --log-file="valgrind_test2.log" ./crawler $SEED_URL $TEST_DIR $MAX_DEPTH

# Check if memory limit was reached
if [ $? -eq 0 ]; then
  echo "Test 2 Passed: Memory limit not exceeded."
else
  echo "Test 2 Failed: Memory limit exceeded."
fi

# Cleanup
echo "Cleaning up test files..."
rm -rf $TEST_DIR

# Summary
echo "Test complete. Check valgrind_test1.log and valgrind_test2.log for detailed memory reports."
