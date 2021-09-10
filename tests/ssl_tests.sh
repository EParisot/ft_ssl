#! /bin/bash

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_res () {
	if [ "$1" = "$2" ]; then
	printf "\t\t\t\t\t\t\t\t\t\t${GREEN}\xE2\x9C\x94\x0a${NC}"
	else
	printf "\t\t\t\t\t\t\t\t\t\t${RED}\xE2\x9D\x8C\x0a${NC}"
	fi
}

print_diff () {
	DIFF=$(diff $1 $2) 
	if [ "$DIFF" == "" ]  ; then
	printf "\t\t\t\t\t\t\t\t\t\t${GREEN}\xE2\x9C\x94\x0a${NC}"
	else
	printf "$DIFF"
	printf "\t\t\t\t\t\t\t\t\t\t${RED}\xE2\x9D\x8C\x0a${NC}"
	fi
}

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
		CMDA=$(echo -n "$S" | ./ft_ssl $H -q);printf "$CMDA\n"
		echo -e "${BLUE}openssl ${H} ${S}${NC}:"
		CMDB=$(echo -n "$S" | openssl $H | sed 's/(stdin)= //');printf "$CMDB\n"
		print_res "$CMDA" "$CMDB"
		echo
	done
done

echo -e "${BLUE}echo -n 'test\x0\x11\x22\x33\x44\x55\x66\x77' | ./ft_ssl md5 -q${NC}:"
CMDA=$(echo -n 'test\x0\x11\x22\x33\x44\x55\x66\x77' | ./ft_ssl md5 -q);printf "$CMDA\n"
echo -e "${BLUE}echo -n 'test\x0\x11\x22\x33\x44\x55\x66\x77' | openssl md5${NC}:"
CMDB=$(echo -n 'test\x0\x11\x22\x33\x44\x55\x66\x77' | openssl md5 | sed 's/(stdin)= //');printf "$CMDB\n"
print_res "$CMDA" "$CMDB"

echo -e "${YELLOW}Reverse BASE64 Tests${NC}:"; echo

BASE64_STRINGS=("" \
			"YWJj" \
			"Ym9uam91cjQy" \
			"YWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYQ==" \
)
for S in ${BASE64_STRINGS[@]}
	do echo -e "${BLUE}./ft_ssl base64 -d ${S}${NC}:"
	CMDA=$(echo "$S" | ./ft_ssl base64 -d -q);printf "$CMDA\n"
	echo -e "${BLUE}openssl base64 -d ${S}${NC}:"
	CMDB=$(echo "$S" | openssl base64 -d);printf "$CMDB\n"
	print_res "$CMDA" "$CMDB"
	echo
done

echo "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec ut eros eu diam facilisis interdum. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Vivamus ipsum diam, malesuada convallis finibus in, pellentesque non dolor. Maecenas tempus urna vel est dapibus, volutpat auctor ex auctor. Phasellus scelerisque leo sodales fermentum hendrerit. Vivamus egestas accumsan lorem, ut ultricies diam pellentesque eu. Aliquam varius nibh sed arcu auctor, eu scelerisque magna ornare.

Aliquam sed urna in diam dignissim mollis. Fusce ultrices, sem sed dapibus congue, arcu risus pulvinar lacus, iaculis euismod turpis neque sed mauris. Aenean molestie consectetur lectus, faucibus tristique eros euismod id. Maecenas consequat, magna ut finibus cursus, sapien mauris eleifend mi, sed consequat est magna eu felis. Nam blandit tempus finibus. Cras tincidunt sagittis nisl. Pellentesque non condimentum nibh, ac pulvinar neque. In rhoncus, turpis et finibus faucibus, est sapien pulvinar massa, a egestas odio eros ac erat. Etiam accumsan, sapien id dapibus maximus, velit ligula tincidunt libero, nec consectetur nibh dolor a purus. Donec eget metus in mi vestibulum rutrum et a tellus. Pellentesque dapibus erat et sem euismod, a varius sapien blandit. Praesent vitae orci sit amet orci interdum malesuada. Phasellus ultricies sed est id ullamcorper. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Nam malesuada suscipit justo, in rutrum libero tempus in. Interdum et malesuada fames ac ante ipsum primis in faucibus.

Nulla est nulla, varius at vulputate eu, eleifend a nunc. Pellentesque elit massa, dictum in neque eget, elementum semper urna. Mauris bibendum elit ut ex tempus viverra. Phasellus consectetur enim id libero viverra cursus. Nulla gravida vehicula eros sit amet euismod. Vestibulum quis sapien mauris. Donec varius purus arcu, vel aliquam sapien porta sed. Mauris quis fringilla turpis. Praesent ante odio, laoreet at tellus in, vestibulum consequat mi. Nullam quis faucibus lorem. Etiam pretium sit amet nisl nec accumsan. Phasellus maximus, massa quis dignissim venenatis, nibh diam maximus sem, sed venenatis sapien quam at lectus. Integer lorem sapien, tristique sit amet arcu in, malesuada placerat lectus. Integer molestie dui in purus sagittis convallis. Praesent molestie id mauris eu consequat.

Ut malesuada felis orci, eget rhoncus ligula mollis ac. Morbi egestas malesuada elit id eleifend. Mauris eros magna, posuere in purus nec, vehicula iaculis est. Proin sodales laoreet lorem, et pretium arcu dictum at. Pellentesque arcu felis, egestas ut ultricies nec, scelerisque non quam. Sed porta iaculis vehicula. Aliquam erat volutpat. Curabitur non metus non lorem blandit commodo quis non sem. Donec turpis augue, ultricies sit amet turpis at, vehicula tincidunt tellus. Aliquam erat volutpat. Integer vitae diam sit amet elit varius facilisis in sit amet velit. Curabitur facilisis tempus aliquam. Praesent ac tortor eget velit aliquet vehicula non eget enim. Aliquam lobortis mollis nulla, vel porttitor massa aliquet in.

Donec a consectetur erat. Nulla aliquet felis at ornare ultrices. Pellentesque lobortis in metus ut rutrum. Curabitur sit amet eros ac justo feugiat vehicula. Proin ut augue feugiat, ultrices nisi sodales, imperdiet orci. Nunc tincidunt sollicitudin pulvinar. Aliquam ac mauris dolor. Sed mauris purus, aliquam nec neque a, dignissim fermentum nulla. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae;" > file


echo -e "${BLUE}./ft_ssl base64 -i file -o test1${NC}:"
./ft_ssl base64 -i file -o test1;
echo -e "${BLUE}openssl base64 -in file -out test2${NC}:"
openssl base64 -in file -out test2;
print_diff test1 test2
echo
echo -e "${BLUE}./ft_ssl base64 -d -i test1 -o out1${NC}:"
./ft_ssl base64 -d -i test1 -o out1;
echo -e "${BLUE}openssl base64 -d -in test2 -out out2${NC}:"
openssl base64 -d -in test2 -out out2;
print_diff test1 test2
echo

echo -e "${YELLOW}DES-ECB${NC}:"; echo

echo -e "${BLUE}echo \"foo bar\" | ./ft_ssl des-ecb -q -a -k 133457799BBCDFF1${NC}"
CMDA=$(echo "foo bar" | ./ft_ssl des-ecb -q -a -k 133457799BBCDFF1);echo $CMDA
echo -e "${BLUE}echo \"foo bar\" | openssl des-ecb -a -K 133457799BBCDFF1${NC}"
CMDB=$(echo "foo bar" | openssl des-ecb -a -K 133457799BBCDFF1);echo $CMDB
print_res "$CMDA" "$CMDB";
echo
echo -e "${BLUE}echo \"O4LusmJIpRn98uF0SSki+A==\" | ./ft_ssl des-ecb -d -k 133457799BBCDFF1 -q -a${NC}"
CMDA=$(echo "O4LusmJIpRn98uF0SSki+A==" | ./ft_ssl des-ecb -d -k 133457799BBCDFF1 -q -a);echo $CMDA
echo -e "${BLUE}echo \"O4LusmJIpRn98uF0SSki+A==\" | openssl des-ecb -d -K 133457799BBCDFF1 -a${NC}"
CMDB=$(echo "O4LusmJIpRn98uF0SSki+A==" | openssl des-ecb -d -K 133457799BBCDFF1 -a);echo $CMDB
print_res "$CMDA" "$CMDB";
echo
echo -e "${BLUE}echo \"one deep secret\" | ./ft_ssl des-ecb -k 133457799BBCDFF1 -q -a${NC}"
CMDA=$(echo "one deep secret" | ./ft_ssl des-ecb -k 133457799BBCDFF1 -q -a);echo $CMDA
echo -e "${BLUE}echo \"one deep secret\" | openssl des-ecb -K 133457799BBCDFF1 -a${NC}"
CMDB=$(echo "one deep secret" | openssl des-ecb -K 133457799BBCDFF1 -a);echo $CMDB
print_res "$CMDA" "$CMDB";
echo
echo -e "${BLUE}echo \"mJWdy4+Oe1fBEj0N3cTox/3y4XRJKSL4\" | ./ft_ssl des-ecb -d -k 133457799BBCDFF1 -q -a${NC}"
CMDA=$(echo "mJWdy4+Oe1fBEj0N3cTox/3y4XRJKSL4" | ./ft_ssl des-ecb -d -k 133457799BBCDFF1 -q -a);echo $CMDA
echo -e "${BLUE}echo \"mJWdy4+Oe1fBEj0N3cTox/3y4XRJKSL4\" | openssl -d des-ecb -K 133457799BBCDFF1 -a${NC}"
CMDB=$(echo "mJWdy4+Oe1fBEj0N3cTox/3y4XRJKSL4" | openssl des-ecb -d -K 133457799BBCDFF1 -a);echo $CMDB
print_res "$CMDA" "$CMDB";

echo
echo -e "${BLUE}./ft_ssl des-ecb -q -k 0011223344556677 -v 0011223344556677 -i file -o test1${NC}"
./ft_ssl des-ecb -q -k 0011223344556677 -v 0011223344556677 -i file -o test1
echo -e "${BLUE}openssl des-ecb -K 0011223344556677 -iv 0011223344556677 -in file -out test2${NC}"
openssl des-ecb -K 0011223344556677 -iv 0011223344556677 -in file -out test2
print_diff test1 test2
echo
echo -e "${BLUE}./ft_ssl des-ecb -d -q -k 0011223344556677 -v 0011223344556677 -o out1 -i test1${NC}"
./ft_ssl des-ecb -d -q -k 0011223344556677 -v 0011223344556677 -o out1 -i test1
echo -e "${BLUE}openssl des-ecb -d -K 0011223344556677 -iv 0011223344556677 -out out2 -in test2${NC}"
openssl des-ecb -d -K 0011223344556677 -iv 0011223344556677 -out out2 -in test2
print_diff out1 out2

echo
echo -e "${BLUE}./ft_ssl des-ecb -a -k 0011223344556677 -v 0011223344556677 -i file -o test1${NC}"
./ft_ssl des-ecb -a -k 0011223344556677 -v 0011223344556677 -i file -o test1
echo -e "${BLUE}openssl des-ecb -a -K 0011223344556677 -iv 0011223344556677 -in file -out test2${NC}"
openssl des-ecb -a -K 0011223344556677 -iv 0011223344556677 -in file -out test2
print_diff test1 test2
echo
echo -e "${BLUE}./ft_ssl des-ecb -d -a -k 0011223344556677 -v 0011223344556677 -o out1 -i test1${NC}"
./ft_ssl des-ecb -d -a -k 0011223344556677 -v 0011223344556677 -o out1 -i test1
echo -e "${BLUE}openssl des-ecb -d -a -K 0011223344556677 -iv 0011223344556677 -out out2 -in test2${NC}"
openssl des-ecb -d -a -K 0011223344556677 -iv 0011223344556677 -out out2 -in test2
print_diff out1 out2
echo

echo
echo -e "${BLUE}./ft_ssl des-ecb -a -k 0011223344556677 -v 0011223344556677 -i file -o test1 ${NC}"
./ft_ssl des-ecb -a -k 0011223344556677 -v 0011223344556677 -i file -o test1 
echo -e "${BLUE}openssl des-ecb -a -K 0011223344556677 -iv 0011223344556677 -in file -out test2${NC}"
openssl des-ecb -a -K 0011223344556677 -iv 0011223344556677 -in file -out test2
print_diff test1 test2
echo
echo -e "${BLUE}./ft_ssl des-ecb -d -a -k 0011223344556677 -v 0011223344556677 -o out1 -i test1${NC}"
./ft_ssl des-ecb -d -a -k 0011223344556677 -v 0011223344556677 -o out1 -i test1
echo -e "${BLUE}openssl des-ecb -d -a -K 0011223344556677 -iv 0011223344556677 -out out2 -in test2${NC}"
openssl des-ecb -d -a -K 0011223344556677 -iv 0011223344556677 -out out2 -in test2
print_diff out1 out2
echo

echo
echo -e "${BLUE}./ft_ssl des-ecb -a -p password -v 0011223344556677 -s 0011223344556677 -i file -o test1 ${NC}"
./ft_ssl des-ecb -a -p password -v 0011223344556677 -s 0011223344556677 -i file -o test1 
echo -e "${BLUE}openssl des-ecb -a -pass pass:password -iv 0011223344556677 -S 0011223344556677 -in file -out test2${NC}"
openssl des-ecb -a -pass pass:password -iv 0011223344556677 -S 0011223344556677 -in file -out test2
print_diff test1 test2
echo
echo -e "${BLUE}./ft_ssl des-ecb -d -a -p password -v 0011223344556677 -s 0011223344556677 -o out1 -i test1${NC}"
./ft_ssl des-ecb -d -a -p password -v 0011223344556677 -s 0011223344556677 -o out1 -i test1
echo -e "${BLUE}openssl des-ecb -d -a -pass pass:password -iv 0011223344556677 -S 0011223344556677 -out out2 -in test2${NC}"
openssl des-ecb -d -a -pass pass:password -iv 0011223344556677 -S 0011223344556677 -out out2 -in test2
print_diff out1 out2
echo

echo
echo -e "${BLUE}./ft_ssl des-ecb -a -p password -s 0011223344556677 -i file -o test1 ${NC}"
./ft_ssl des-ecb -a -p password -s 0011223344556677 -i file -o test1 
echo -e "${BLUE}openssl des-ecb -a -pass pass:password -S 0011223344556677 -in file -out test2${NC}"
openssl des-ecb -a -pass pass:password -S 0011223344556677 -in file -out test2
print_diff test1 test2
echo
echo -e "${BLUE}./ft_ssl des-ecb -d -a -p password -s 0011223344556677 -o out1 -i test1${NC}"
./ft_ssl des-ecb -d -a -p password -s 0011223344556677 -o out1 -i test1
echo -e "${BLUE}openssl des-ecb -d -a -pass pass:password -S 0011223344556677 -out out2 -in test2${NC}"
openssl des-ecb -d -a -pass pass:password -S 0011223344556677 -out out2 -in test2
print_diff out1 out2
echo

echo -e "${BLUE}openssl des-ecb -a -pass pass:password -in file -out test2${NC}"
openssl des-ecb -a -pass pass:password -in file -out test1
echo -e "${BLUE}./ft_ssl des-ecb -d -a -p password -o out1 -i test1${NC}"
./ft_ssl des-ecb -d -a -p password -o out1 -i test1
echo -e "${BLUE}openssl des-ecb -d -a -pass pass:password -out out2 -in test2${NC}"
openssl des-ecb -d -a -pass pass:password -out out2 -in test1
print_diff out1 out2
echo

echo -e "${YELLOW}DES-CBC${NC}:"; echo

echo -e "${BLUE}echo \"foo bar\" | ./ft_ssl des-cbc -q -a -k 133457799BBCDFF1 -v 133457799BBCDFF1${NC}"
CMDA=$(echo "foo bar" | ./ft_ssl des-cbc -q -a -k 133457799BBCDFF1 -v 133457799BBCDFF1);echo $CMDA
echo -e "${BLUE}echo \"foo bar\" | openssl des-cbc -a -K 133457799BBCDFF1 -iv 133457799BBCDFF1${NC}"
CMDB=$(echo "foo bar" | openssl des-cbc -a -K 133457799BBCDFF1 -iv 133457799BBCDFF1);echo $CMDB
print_res "$CMDA" "$CMDB";
echo
echo -e "${BLUE}echo \"czo9ml0T2SSYtazEJhwzVw==\" | ./ft_ssl des-cbc -q -a -d -k 133457799BBCDFF1 -v 133457799BBCDFF1${NC}"
CMDA=$(echo "czo9ml0T2SSYtazEJhwzVw==" | ./ft_ssl des-cbc -q -a -d -k 133457799BBCDFF1 -v 133457799BBCDFF1);echo $CMDA
echo -e "${BLUE}echo \"czo9ml0T2SSYtazEJhwzVw==\" | openssl des-cbc -d -a -K 133457799BBCDFF1 -iv 133457799BBCDFF1${NC}"
CMDB=$(echo "czo9ml0T2SSYtazEJhwzVw==" | openssl des-cbc -d -a -K 133457799BBCDFF1 -iv 133457799BBCDFF1);echo $CMDB
print_res "$CMDA" "$CMDB";
echo
echo -e "${BLUE}echo \"one deep secret\" | ./ft_ssl des-cbc -q -a -k 6162636461626364 -v 0011223344556677${NC}"
CMDA=$(echo "one deep secret" | ./ft_ssl des-cbc -q -a -k 6162636461626364 -v 0011223344556677);echo $CMDA
echo -e "${BLUE}echo \"one deep secret\" | openssl des-cbc -a -K 6162636461626364 -iv 0011223344556677${NC}"
CMDB=$(echo "one deep secret" | openssl des-cbc -a -K 6162636461626364 -iv 0011223344556677);echo $CMDB
print_res "$CMDA" "$CMDB";
echo
echo -e "${BLUE}echo \"zqYWONX68rWNxl7msIdGC67Uh2HfVEBo\" | ./ft_ssl des-cbc -q -d -a -k 6162636461626364 -v 0011223344556677${NC}"
CMDA=$(echo "zqYWONX68rWNxl7msIdGC67Uh2HfVEBo" | ./ft_ssl des-cbc -q -d -a -k 6162636461626364 -v 0011223344556677);echo $CMDA
echo -e "${BLUE}echo \"zqYWONX68rWNxl7msIdGC67Uh2HfVEBo\" | openssl des-cbc -d -a -K 6162636461626364 -iv 0011223344556677${NC}"
CMDB=$(echo "zqYWONX68rWNxl7msIdGC67Uh2HfVEBo" | openssl des-cbc -d -a -K 6162636461626364 -iv 0011223344556677);echo $CMDB
print_res "$CMDA" "$CMDB";

echo
echo -e "${BLUE}./ft_ssl des-cbc -q -k 0011223344556677 -v 0011223344556677 -i file -o test1${NC}"
./ft_ssl des-cbc -q -k 0011223344556677 -v 0011223344556677 -i file -o test1
echo -e "${BLUE}openssl des-cbc -K 0011223344556677 -iv 0011223344556677 -in file -out test2${NC}"
openssl des-cbc -K 0011223344556677 -iv 0011223344556677 -in file -out test2
print_diff test1 test2
echo
echo -e "${BLUE}./ft_ssl des-cbc -d -q -k 0011223344556677 -v 0011223344556677 -o out1 -i test1${NC}"
./ft_ssl des-cbc -d -q -k 0011223344556677 -v 0011223344556677 -o out1 -i test1
echo -e "${BLUE}openssl des-cbc -d -K 0011223344556677 -iv 0011223344556677 -out out2 -in test2${NC}"
openssl des-cbc -d -K 0011223344556677 -iv 0011223344556677 -out out2 -in test2
print_diff out1 out2

echo
echo -e "${BLUE}./ft_ssl des-cbc -a -k 0011223344556677 -v 0011223344556677 -i file -o test1 ${NC}"
./ft_ssl des-cbc -a -k 0011223344556677 -v 0011223344556677 -i file -o test1 
echo -e "${BLUE}openssl des-cbc -a -K 0011223344556677 -iv 0011223344556677 -in file -out test2${NC}"
openssl des-cbc -a -K 0011223344556677 -iv 0011223344556677 -in file -out test2
print_diff test1 test2
echo
echo -e "${BLUE}./ft_ssl des-cbc -d -a -k 0011223344556677 -v 0011223344556677 -o out1 -i test1${NC}"
./ft_ssl des-cbc -d -a -k 0011223344556677 -v 0011223344556677 -o out1 -i test1
echo -e "${BLUE}openssl des-cbc -d -a -K 0011223344556677 -iv 0011223344556677 -out out2 -in test2${NC}"
openssl des-cbc -d -a -K 0011223344556677 -iv 0011223344556677 -out out2 -in test2
print_diff out1 out2
echo

echo
echo -e "${BLUE}./ft_ssl des-cbc -a -p password -v 0011223344556677 -s 0011223344556677 -i file -o test1 ${NC}"
./ft_ssl des-cbc -a -p password -v 0011223344556677 -s 0011223344556677 -i file -o test1 
echo -e "${BLUE}openssl des-cbc -a -pass pass:password -iv 0011223344556677 -S 0011223344556677 -in file -out test2${NC}"
openssl des-cbc -a -pass pass:password -iv 0011223344556677 -S 0011223344556677 -in file -out test2
print_diff test1 test2
echo
echo -e "${BLUE}./ft_ssl des-cbc -d -a -p password -v 0011223344556677 -s 0011223344556677 -o out1 -i test1${NC}"
./ft_ssl des-cbc -d -a -p password -v 0011223344556677 -s 0011223344556677 -o out1 -i test1
echo -e "${BLUE}openssl des-cbc -d -a -pass pass:password -iv 0011223344556677 -S 0011223344556677 -out out2 -in test2${NC}"
openssl des-cbc -d -a -pass pass:password -iv 0011223344556677 -S 0011223344556677 -out out2 -in test2
print_diff out1 out2
echo

echo -e "${BLUE}./ft_ssl des-cbc -a -p password -s 0011223344556677 -i file -o test1 ${NC}"
./ft_ssl des-cbc -a -p password -s 0011223344556677 -i file -o test1 
echo -e "${BLUE}openssl des-cbc -a -pass pass:password -S 0011223344556677 -in file -out test2${NC}"
openssl des-cbc -a -pass pass:password -S 0011223344556677 -in file -out test2
print_diff test1 test2
echo
echo -e "${BLUE}./ft_ssl des-cbc -d -a -p password -s 0011223344556677 -o out1 -i test1${NC}"
./ft_ssl des-cbc -d -a -p password -s 0011223344556677 -o out1 -i test1
echo -e "${BLUE}openssl des-cbc -d -a -pass pass:password -S 0011223344556677 -out out2 -in test2${NC}"
openssl des-cbc -d -a -pass pass:password -S 0011223344556677 -out out2 -in test2
print_diff out1 out2
echo

echo -e "${BLUE}openssl des-cbc -a -pass pass:password -in file -out test2${NC}"
openssl des-cbc -a -pass pass:password -in file -out test1
echo -e "${BLUE}./ft_ssl des-cbc -d -a -p password -o out1 -i test1${NC}"
./ft_ssl des-cbc -d -a -p password -o out1 -i test1
echo -e "${BLUE}openssl des-cbc -d -a -pass pass:password -out out2 -in test2${NC}"
openssl des-cbc -d -a -pass pass:password -out out2 -in test1
print_diff out1 out2
echo

rm file test1 test2 out1 out2