#! /bin/bash

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${GREEN}FT_SSL Tests:${NC}"; echo
echo -e "${YELLOW}Compiling...${NC}"
make
echo

echo -e "${YELLOW}Simple Tests${NC}:"; echo

HASHES=(md5 sha224 sha256 base64)
TEST_STRINGS=("" \
			"abc" \
			"bonjour42" \
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" \
)

for H in ${HASHES[@]} 
	do for S in ${TEST_STRINGS[@]}
		do echo -e "${BLUE}./ft_ssl ${H} ${S}${NC}:"
		echo -n "$S" | ./ft_ssl $H; echo -e "${BLUE}openssl ${H} ${S}${NC}:"
		echo -n "$S" | openssl $H | sed 's/(stdin)= //'
		echo
	done
done

echo -e "${YELLOW}Decode Tests${NC}:"; echo

HASHES=(base64)
base64_STRINGS=("" \
			"YWJj" \
			"Ym9uam91cjQy" \
			"YWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYQ==" \
)

for H in ${HASHES[@]}
	do var=${H}_STRINGS[@]
	for S in ${!var}
		do echo -e "${BLUE}./ft_ssl ${H} -d ${S}${NC}:"
		echo "$S" | ./ft_ssl $H -d; echo
		echo -e "${BLUE}openssl ${H} -d ${S}${NC}:"
		echo "$S" | openssl $H -d ; echo
		echo
	done
done