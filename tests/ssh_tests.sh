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
		echo "$S" | ./ft_ssl $H -d -q; echo
		echo -e "${BLUE}openssl ${H} -d ${S}${NC}:"
		echo "$S" | openssl $H -d ; echo
		echo
	done
done


echo -e "${YELLOW}Subject Examples :${NC}"
echo

echo -e "${BLUE}\"42 is nice\" | ./ft_ssl md5${NC}"
echo "42 is nice" | ./ft_ssl md5
echo

echo -e "${BLUE}\"42 is nice\" | ./ft_ssl md5 -p${NC}"
echo "42 is nice" | ./ft_ssl md5 -p
echo


echo -e "${BLUE}echo \"Pity the living.\" | ./ft_ssl md5 -q -r${NC}"
echo "Pity the living." | ./ft_ssl md5 -q -r
echo

echo -e "${BLUE}echo \"And above all,\" > file${NC}"
echo "And above all," > file
echo -e "${BLUE}./ft_ssl md5 file${NC}"
./ft_ssl md5 file
echo -e "${BLUE}./ft_ssl md5 -r file${NC}"
./ft_ssl md5 -r file
echo

echo -e "${BLUE}./ft_ssl md5 -s \"pity those that aren't following baerista on spotify.\"${NC}"
./ft_ssl md5 -s "pity those that aren't following baerista on spotify."
echo

echo -e "${BLUE}echo \"be sure to handle edge cases carefully\" | ./ft_ssl md5 -p file${NC}"
echo "be sure to handle edge cases carefully" | ./ft_ssl md5 -p file
echo

echo -e "${BLUE}echo \"some of this will not make sense at first\" | ./ft_ssl md5 file${NC}"
echo "some of this will not make sense at first" | ./ft_ssl md5 file
echo

echo -e "${BLUE}echo \"but eventually you will understand\" | ./ft_ssl md5 -p -r file${NC}"
echo "but eventually you will understand" | ./ft_ssl md5 -p -r file
echo

echo -e "${BLUE}echo \"GL HF let's go\" | ./ft_ssl md5 -p -s \"foo\" file${NC}"
echo "GL HF let's go" | ./ft_ssl md5 -p -s "foo" file
echo

echo -e "${BLUE}echo \"one more thing\" | ./ft_ssl md5 -r -p -s \"foo\" file -s \"bar\"${NC}"
echo "one more thing" | ./ft_ssl md5 -r -p -s "foo" file -s "bar"
echo

echo -e "${BLUE}echo \"just to be extra clear\" | ./ft_ssl md5 -r -q -p -s \"foo\" file${NC}"
echo "just to be extra clear" | ./ft_ssl md5 -r -q -p -s "foo" file