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
		do echo -e "${BLUE}./ft_ssl ${H} -q ${S}${NC}:"
		echo -n "$S" | ./ft_ssl $H -q; echo -e "${BLUE}openssl ${H} ${S}${NC}:"
		echo -n "$S" | openssl $H | sed 's/(stdin)= //'
		echo
	done
done

echo -e "${YELLOW}Reverse BASE64 Tests${NC}:"; echo

BASE64_STRINGS=("" \
			"YWJj" \
			"Ym9uam91cjQy" \
			"YWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYQ==" \
)
for S in ${BASE64_STRINGS[@]}
	do echo -e "${BLUE}./ft_ssl base64 -d ${S}${NC}:"
	echo "$S" | ./ft_ssl base64 -d -q; echo
	echo -e "${BLUE}openssl base64 -d ${S}${NC}:"
	echo "$S" | openssl base64 -d ; echo
	echo
done

echo "42 is awesome !" > file
echo -e "${YELLOW}DES-ECB${NC}:"; echo
echo -e "${BLUE}echo \"foo bar\" | ./ft_ssl des-ecb -q -a -k 133457799BBCDFF1${NC}"
echo "foo bar" | ./ft_ssl des-ecb -q -a -k 133457799BBCDFF1
echo -e "${BLUE}echo \"foo bar\" | openssl des-ecb -a -K 133457799BBCDFF1${NC}"
echo "foo bar" | openssl des-ecb -a -K 133457799BBCDFF1
echo
echo -e "${BLUE}echo \"O4LusmJIpRn98uF0SSki+A==\" | ./ft_ssl des-ecb -d -k 133457799BBCDFF1 -q -a${NC}"
echo "O4LusmJIpRn98uF0SSki+A==" | ./ft_ssl des-ecb -d -k 133457799BBCDFF1 -q -a
echo -e "${BLUE}echo \"O4LusmJIpRn98uF0SSki+A==\" | openssl -d des-ecb -K 133457799BBCDFF1 -a${NC}"
echo "O4LusmJIpRn98uF0SSki+A==" | openssl des-ecb -d -K 133457799BBCDFF1 -a
echo
echo -e "${BLUE}echo \"one deep secret\" | ./ft_ssl des-ecb -k 133457799BBCDFF1 -q -a${NC}"
echo "one deep secret" | ./ft_ssl des-ecb -k 133457799BBCDFF1 -q -a
echo -e "${BLUE}echo \"one deep secret\" | openssl des-ecb -K 133457799BBCDFF1 -a${NC}"
echo "one deep secret" | openssl des-ecb -K 133457799BBCDFF1 -a
echo
echo -e "${BLUE}echo \"mJWdy4+Oe1fBEj0N3cTox/3y4XRJKSL4\" | ./ft_ssl des-ecb -d -k 133457799BBCDFF1 -q -a${NC}"
echo "mJWdy4+Oe1fBEj0N3cTox/3y4XRJKSL4" | ./ft_ssl des-ecb -d -k 133457799BBCDFF1 -q -a
echo -e "${BLUE}echo \"mJWdy4+Oe1fBEj0N3cTox/3y4XRJKSL4\" | openssl -d des-ecb -K 133457799BBCDFF1 -a${NC}"
echo "mJWdy4+Oe1fBEj0N3cTox/3y4XRJKSL4" | openssl des-ecb -d -K 133457799BBCDFF1 -a
echo
echo -e "${BLUE}./ft_ssl des-ecb -q -d -k 6162636461626364 -v 0011223344556677 -i file -o test1${NC}"
./ft_ssl des-ecb -q -d -k 6162636461626364 -v 0011223344556677 -i file -o test1
echo -e "${BLUE}openssl des-ecb -d -K 6162636461626364 -iv 0011223344556677 -in file -out test2 -nopad${NC}"
openssl des-ecb -d -K 6162636461626364 -iv 0011223344556677 -in file -out test2 -nopad
echo
echo -e "${BLUE}./ft_ssl des-ecb -q -k 6162636461626364 -v 0011223344556677 -o file -i test1${NC}"
./ft_ssl des-ecb -q -k 6162636461626364 -v 0011223344556677 -o file -i test1
echo -e "${BLUE}openssl des-ecb -K 6162636461626364 -iv 0011223344556677 -out file -in test2 -nopad${NC}"
openssl des-ecb -K 6162636461626364 -iv 0011223344556677 -out file -in test2 -nopad
diff test1 test2
echo

echo -e "${YELLOW}DES-CBC${NC}:"; echo
echo -e "${BLUE}echo \"foo bar\" | ./ft_ssl des-cbc -q -a -k 133457799BBCDFF1 -v 133457799BBCDFF1${NC}"
echo "foo bar" | ./ft_ssl des-cbc -q -a -k 133457799BBCDFF1 -v 133457799BBCDFF1
echo -e "${BLUE}echo \"foo bar\" | openssl des-cbc -a -K 133457799BBCDFF1 -iv 133457799BBCDFF1${NC}"
echo "foo bar" | openssl des-cbc -a -K 133457799BBCDFF1 -iv 133457799BBCDFF1
echo
echo -e "${BLUE}echo \"czo9ml0T2SSYtazEJhwzVw==\" | ./ft_ssl des-cbc -q -a -d -k 133457799BBCDFF1 -v 133457799BBCDFF1${NC}"
echo "czo9ml0T2SSYtazEJhwzVw==" | ./ft_ssl des-cbc -q -a -d -k 133457799BBCDFF1 -v 133457799BBCDFF1
echo -e "${BLUE}echo \"czo9ml0T2SSYtazEJhwzVw==\" | openssl des-cbc -d -a -K 133457799BBCDFF1 -iv 133457799BBCDFF1${NC}"
echo "czo9ml0T2SSYtazEJhwzVw==" | openssl des-cbc -d -a -K 133457799BBCDFF1 -iv 133457799BBCDFF1
echo
echo -e "${BLUE}echo \"one deep secret\" | ./ft_ssl des-cbc -q -a -k 6162636461626364 -v 0011223344556677${NC}"
echo "one deep secret" | ./ft_ssl des-cbc -q -a -k 6162636461626364 -v 0011223344556677
echo -e "${BLUE}echo \"one deep secret\" | openssl des-cbc -a -K 6162636461626364 -iv 0011223344556677${NC}"
echo "one deep secret" | openssl des-cbc -a -K 6162636461626364 -iv 0011223344556677
echo
echo -e "${BLUE}echo \"zqYWONX68rWNxl7msIdGC67Uh2HfVEBo\" | ./ft_ssl des-cbc -q -d -a -k 6162636461626364 -v 0011223344556677${NC}"
echo "zqYWONX68rWNxl7msIdGC67Uh2HfVEBo" | ./ft_ssl des-cbc -q -d -a -k 6162636461626364 -v 0011223344556677
echo -e "${BLUE}echo \"zqYWONX68rWNxl7msIdGC67Uh2HfVEBo\" | openssl des-cbc -d -a -K 6162636461626364 -iv 0011223344556677${NC}"
echo "zqYWONX68rWNxl7msIdGC67Uh2HfVEBo" | openssl des-cbc -d -a -K 6162636461626364 -iv 0011223344556677
echo
echo -e "${BLUE}./ft_ssl des-cbc -q -d -k 6162636461626364 -v 0011223344556677 -i file -o test1${NC}"
./ft_ssl des-cbc -q -d -k 6162636461626364 -v 0011223344556677 -i file -o test1
echo -e "${BLUE}openssl des-cbc -d -K 6162636461626364 -iv 0011223344556677 -in file -out test2 -nopad${NC}"
openssl des-cbc -d -K 6162636461626364 -iv 0011223344556677 -in file -out test2 -nopad
echo
echo -e "${BLUE}./ft_ssl des-cbc -q -k 6162636461626364 -v 0011223344556677 -o file -i test1${NC}"
./ft_ssl des-cbc -q -k 6162636461626364 -v 0011223344556677 -o file -i test1
echo -e "${BLUE}openssl des-cbc -K 6162636461626364 -iv 0011223344556677 -out file -in test2 -nopad${NC}"
openssl des-cbc -K 6162636461626364 -iv 0011223344556677 -out file -in test2
diff test1 test2
echo
rm file test1 test2