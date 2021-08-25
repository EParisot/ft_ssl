/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

void			print_help(int usage)
{
	if (usage)
	{
		printf("usage: ./ft_ssl [hash] [opt] [string]\n");
		printf("General options: \n");
		printf("		-p		Read from STDIN and repeat input on STDOUT\n");
		printf("		-q		Quiet mode\n");
		printf("		-s		Read next arg as a string\n");
		printf("		-hex	Read next arg as an hex string\n");
		printf("		-r		Reverse output order\n");
		printf("		-i		Read from input file (specified as next arg)\n");
		printf("		-o		Output to file (specified as next arg)\n");
		printf("		-h		Show this Help\n");

		printf("Crypto options: \n");
		printf("		-a		Encode/Decode to/from base64 before encryption/decryption\n");
		printf("		-e		Encode (default)\n");
		printf("		-d		Decode\n");
		printf("		-p		Password\n");
		printf("		-k		Key\n");
		printf("		-s		Salt\n");
		printf("		-v		IV\n");
	}
	printf("\nCommands:\n");
	printf("		md5\n");
	printf("		sha224\n");
	printf("		sha256\n");
	printf("		base64\n");
	printf("		des (default to ecb mode)\n");
	printf("		des_ecb\n");
	printf("		des_cbc\n");
}

void			del(void *addr, size_t size)
{
	(void)size;
	if (((t_string *)addr)->string)
		free(((t_string *)addr)->string);
	if (((t_string *)addr)->source)
		free(((t_string *)addr)->source);
	free(addr);
}

void			clean_data(t_data *data)
{
	if (data && data->pass)
		free(data->pass);
	if (data && data->strings)
		ft_lstdel(&data->strings, del);
	if (data && data->hash)
		free(data->hash);
	free(data);
}

uint32_t		rot_r(uint32_t x, uint32_t n)
{
	return ((x >> n) | (x << (32 - n)));
}

uint32_t		rot_l(uint32_t x, uint32_t n)
{
	return ((x << n) | (x >> (32 - n)));
}

char			*rot_str_l(char *str, int n)
{
	for (int i = 0; i < n; i++)
	{
		char mem = str[0];
		for (size_t i = 0; i < strlen(str) - 1; i++)
		{
			str[i] = str[i + 1];
		}
		str[strlen(str) - 1] = mem;
	}
	return str;
}

void			xor_bin(char *a, char *b)
{
	for (int i = 0; i < 32; i++)
	{
		b[i] = ((b[i] - '0') ^ (a[i] - '0')) + '0';
	}
}

void			xor(unsigned char *a, unsigned char *b)
{
	for (int i = 0; i < 8; i++)
	{
		b[i] ^= a[i];
	}
}

void 			print_hex(unsigned char *hex, size_t size)
{
	printf("0x");
	for (size_t i = 0; i < size; i++)
	{
		printf("%02x", hex[i]);
	}
}

int 				bitstoint(char *binstr)
{
	int res = 0;
	int i = 0;
	while (binstr[i] != '\0')
	{
		res *= 2;
		if (binstr[i] == '1') res += 1;
		++i;
	}
	return res;
}

char 				*inttobits(int num, char *bits, size_t size)
{
	int i = 0;

	while (size > 0)
	{
		if (num > 0)
		{
			bits[i] = (num % 2) ? '1' : '0';
			++i;
			--size;
			num /= 2;
		}
		else
		{
			bits[i] = '0';
			++i;
			--size;
		}
	}
	return ft_strrev(bits);
}

char 			*str_to_bin(char *str, char *binstr)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			binstr[i * 8 + j] = '0' + ((str[i] >> (7 - j)) & 1);
		}
	}
	return binstr;
}

char 			*bin_to_str(char *binstr, char *str)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			str[i] += (binstr[i * 8 + j] - '0') * (1 << (7 - j));
		}
	}
	return str;
}

int 			random_value(unsigned char *r, size_t size)
{
	int fd = 0;

	if ((fd = open("/dev/urandom", O_RDONLY)) < 0)
		return -1;
	if (read(fd, r, size) < 0)
		return -1;
	for (size_t i = 0; i < size; i++)
	{
		if (r[i] >= 255)
			r[i] = r[i] % 255;
	}
	close(fd);
	return 0;
}