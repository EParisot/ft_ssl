#! /bin/bash

HASHES=(md5 sha256)

TEST_STRINGS=("" "abc" "bonjour42" "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")

make

echo "Simple Tests :"
for H in ${HASHES[@]}; do for S in ${TEST_STRINGS[@]}; do echo -n " $S" | ./ft_ssl $H; echo -n " $S" | openssl $H; done; done

echo "Valgrind Tests :"
for H in ${HASHES[@]}; do for S in ${TEST_STRINGS[@]}; do echo -n " $S" | valgrind ./ft_ssl $H -p -s "s_test" README.md; done; done