# testing.sh
# Author: Godwin Kipyatich Kangor
# Date: Oct 28, 2024
# Description: Bash script to test indexer.c

# Testing with invalid arguments
echo -e "TESTING WITH INVALID PAGEDIRECTORY [non existent path]"
./indexer ../tem/letters index.out
echo -e "-----------------------------------------------"

echo -e "TESTING WITH INVALID PAGEDIRECTORY [not a crawler directory]"
./indexer ../lab3-GodwinKangor/bag index.out
echo -e "-----------------------------------------------"

# echo -e "TESTING WITH INVALID INDEXFILENAME"
# ./indexer ../data/letters indexx.xt
# echo -e "-----------------------------------------------"

# Testing with no argument
echo -e "TESTING WITH NO ARGUMENTS\n"
./indexer 
echo -e "-----------------------------------------------"

# Testing with one argument
echo -e "TESTING WITH 1 ARGUMENTS\n"
./indexer ../data/letters 
echo -e "-----------------------------------------------"

# Testing with three or more arguments argument
echo -e "TESTING WITH 3 OR MORE ARGUMENTS\n"
./indexer ~/cs50-dev/shared/students_files/tse/output/letters-3 ../data/letters.index ../data/letters
echo -e "-----------------------------------------------"

# Testing with valid arguments
echo -e "TESTING WITH VALID ARGUMENTS [My crawler output]\n"
./indexer /thayerfs/home/f006vzt/cs50-dev/tse-GodwinKangor/data/letters-3  index.out
echo -e "-----------------------------------------------"

echo -e "TESTING WITH VALID ARGUMENTS [shared file output]\n"
./indexer ~/cs50-dev/shared/students_files/tse/output/letters-3 index.out
echo -e "-----------------------------------------------"


# indextest test cases
echo -e "TESTING ./indexertest WITH index.out\n"
./indextest index.out index.copy
echo -e "-----------------------------------------------"

# valgrind testing
echo -e "TESTING VALGRIND: indexer\n"
valgrind ./indexer ~/cs50-dev/shared/students_files/tse/output/letters-3 index.out
echo -e "-----------------------------------------------"

echo -e "TESTING VALGRIND: indextest\n"
valgrind ./indextest index.out index.copy
echo -e "-----------------------------------------------"