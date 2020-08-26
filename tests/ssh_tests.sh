#! /bin/bash

HASHES=(md5 sha224 sha256)

TEST_STRINGS=("" "abc" "bonjour42" "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")

make

echo "Simple Tests :"
for H in ${HASHES[@]}; do for S in ${TEST_STRINGS[@]}; do echo -n " $S" | ./ft_ssl $H; echo -n " $S" | openssl $H; done; done

echo "Valgrind Tests :"
for H in ${HASHES[@]}; do for S in ${TEST_STRINGS[@]}; do echo -n " $S" | valgrind ./ft_ssl $H -p -s "s_test" README.md; done; done

echo "Subject Examples :"

echo '> echo "pickle rick" | openssl md5'
echo "pickle rick" | openssl md5

echo "> "

echo '> echo "pickle rick" | ./ft_ssl md5'
echo "pickle rick" | ./ft_ssl md5

echo "> "

echo '> echo "Do not pity the dead, Harry." | ./ft_ssl md5 -p'
echo "Do not pity the dead, Harry." | ./ft_ssl md5 -p

echo "> "

echo '> echo "Pity the living." | ./ft_ssl md5 -q -r'
echo "Pity the living." | ./ft_ssl md5 -q -r

echo "> "

echo '> echo "And above all," > file'
echo "And above all," > file
echo "> ./ft_ssl md5 file"
./ft_ssl md5 file
echo '> ./ft_ssl md5 -r file'
./ft_ssl md5 -r file

echo "> "

echo "> ./ft_ssl md5 -s 'pity those that aren't following baerista on spotify.'"
./ft_ssl md5 -s "pity those that aren't following baerista on spotify."

echo "> "

echo '> echo "be sure to handle edge cases carefully" | ./ft_ssl md5 -p file'
echo "be sure to handle edge cases carefully" | ./ft_ssl md5 -p file

echo "> "

echo '> echo "some of this will not make sense at first" | ./ft_ssl md5 file'
echo "some of this will not make sense at first" | ./ft_ssl md5 file

echo "> "

echo '> echo "but eventually you will understand" | ./ft_ssl md5 -p -r file'
echo "but eventually you will understand" | ./ft_ssl md5 -p -r file

echo "> "

echo "> echo 'GL HF let's go' | ./ft_ssl md5 -p -s 'foo' file"
echo "GL HF let's go" | ./ft_ssl md5 -p -s "foo" file

echo "> " 

echo '> echo "one more thing" | ./ft_ssl md5 -r -p -s "foo" file -s "bar"'
echo "one more thing" | ./ft_ssl md5 -r -p -s "foo" file -s "bar"

echo "> "

echo '> echo "just to be extra clear" | ./ft_ssl md5 -r -q -p -s "foo" file'
echo "just to be extra clear" | ./ft_ssl md5 -r -q -p -s "foo" file

rm -rf file