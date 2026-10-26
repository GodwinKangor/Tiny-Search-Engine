# valgrind.sh
# Author: Godwin Kangor
# Date: Nov 3, 2024
# Description: Bash script to test querier.c (memory allocation)

echo "home And for" | valgrind ./querier /thayerfs/home/f006vzt/cs50-dev/tse-GodwinKangor/data/letters-3 /thayerfs/home/f006vzt/cs50-dev/tse-GodwinKangor/indexer/index.out
