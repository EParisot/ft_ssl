#! /bin/bash

HASHES=(md5 sha224 sha256 base64)

TEST_STRINGS=("" "abc" "bonjour42" "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")

make

echo "Simple Tests :"
for H in ${HASHES[@]}; do for S in ${TEST_STRINGS[@]}; do echo -n " $S" | ./ft_ssl $H; echo -n " $S" | openssl $H; done; done