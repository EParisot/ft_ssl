#! /bin/bash

make

HASHES=(md5 sha224 sha256 base64)
TEST_STRINGS=("" "abc" "bonjour42" "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
echo "Simple Tests :"
for H in ${HASHES[@]}; do for S in ${TEST_STRINGS[@]}; do echo -n "$S" | ./ft_ssl $H; echo -n "$S" | openssl $H; done; done

HASHES=(base64)
base64_STRINGS=("" "YWJj" "Ym9uam91cjQy" "YWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYQ==")
echo "reverse Tests :"
for H in ${HASHES[@]}; do var=${H}_STRINGS[@]; for S in ${!var}; do echo -n "$S" | ./ft_ssl $H -d; echo "$S" | openssl $H -d ; echo; done; done