/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_ecb.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 17:12:42 by eparisot          #+#    #+#             */
/*   Updated: 2021/08/07 17:12:42 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

//http://page.math.tu-berlin.de/~kant/teaching/hess/krypto-ws2006/des.htm

void 				preprocess_key(t_data *data, char *key)
{
	int 		kperm[56] = {57, 49, 41, 33, 25, 17, 9,
							1, 58, 50, 42, 34, 26, 18,
							10, 2, 59, 51, 43, 35, 27,
							19, 11, 3, 60, 52, 44, 36,
							63, 55, 47, 39, 31, 23, 15,
							7, 62, 54, 46, 38, 30, 22,
							14, 6, 61, 53, 45, 37, 29,
							21, 13, 5, 28, 20, 12, 4};
	char k[65];
	// convert key to binary string
	bzero(k, 65);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			k[i * 8 + j] = '0' + ((data->key[i] >> (7 - j)) & 1);
		}
	}
	// permute from kperm array
	for (int i = 0; i < 56; i++)
	{
		key[i] = k[kperm[i] - 1];
	}
}

int 				subkeys_routine(char *key, char **keys)
{
	int key_comp[48] = {14, 17, 11, 24, 1, 5,
						3, 28, 15, 6, 21, 10,
						23, 19, 12, 4, 26, 8,
						16, 7, 27, 20, 13, 2,
						41, 52, 31, 37, 47, 55,
						30, 40, 51, 45, 33, 48,
						44, 49, 39, 56, 34, 53,
						46, 42, 50, 36, 29, 32};
	char l_key[29]; 
	char r_key[29];
	char *l_keys[16];
	char *r_keys[16];
	char **key_pairs[2] = {l_keys, r_keys};
	char *concat_keys[16];

	// split key in 2 parts
	bzero(l_key, 29);
	bzero(r_key, 29);
	ft_strncpy(l_key, key, 28);
	ft_strncpy(r_key, key + 28, 28);

	// rotate key pairs
	for (int i = 0; i < 16; i++)
	{
		if ((key_pairs[0][i] = malloc(29)) == NULL)
			return -1;
		if ((key_pairs[1][i] = malloc(29)) == NULL)
			return -1;
		bzero(key_pairs[0][i], 29);
		bzero(key_pairs[1][i], 29);
		if (i == 0 || i == 1 || i == 8 || i == 15)
		{
			if (i == 0)
			{
				ft_strcpy(key_pairs[0][i], rot_str_l(l_key, 1));
				ft_strcpy(key_pairs[1][i], rot_str_l(r_key, 1));
			}
			else
			{
				ft_strcpy(key_pairs[0][i], rot_str_l(key_pairs[0][i-1], 1));
				ft_strcpy(key_pairs[1][i], rot_str_l(key_pairs[1][i-1], 1));
			}
		}
		else
		{
			ft_strcpy(key_pairs[0][i], rot_str_l(key_pairs[0][i-1], 2));
			ft_strcpy(key_pairs[1][i], rot_str_l(key_pairs[1][i-1], 2));
		}

		// concat rotated key pairs
		if ((concat_keys[i] = malloc(57)) == NULL)
			return -1;
		bzero(concat_keys[i], 57);
		ft_strncpy(concat_keys[i], key_pairs[0][i], 28);
		ft_strncat(concat_keys[i], key_pairs[1][i], 28);

		// permute from key_comp array
		if ((keys[i] = malloc(49)) == NULL)
			return -1;
		bzero(keys[i], 49);
		for (int j = 0; j < 48; j++)
		{
			keys[i][j] = concat_keys[i][key_comp[j] - 1];
		}
	}
	for (int i = 0; i < 16; i++)
	{
		free(key_pairs[0][i]);
		free(key_pairs[1][i]);
		free(concat_keys[i]);
	}
	return 0;
}

char 				*preprocess_message(char *str, size_t *str_len)
{
	char *message;
	size_t msg_len = ft_strlen(str);

	// padd message
	while (msg_len % 8 != 0)
	{
		msg_len++;
	}
	if ((message = malloc(msg_len + 1)) == NULL)
		return NULL;
	bzero(message, msg_len + 1);
	ft_strcpy(message, str);
	*str_len = msg_len;
	return message;
}

void 				des_clean(char **keys, char *message)
{
	for (int i = 0; i < 16; i++)
	{
		free(keys[i]);
	}
	if (message != NULL)
		free(message);
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

void  				f_function(char *r, char *key, char *res)
{
	int exp_d[48] = {32, 1, 2, 3, 4, 5, 4, 5,
					6, 7, 8, 9, 8, 9, 10, 11,
					12, 13, 12, 13, 14, 15, 16, 17,
					16, 17, 18, 19, 20, 21, 20, 21,
					22, 23, 24, 25, 24, 25, 26, 27,
					28, 29, 28, 29, 30, 31, 32, 1};
	char 	expanded_r[49];
	bzero(expanded_r, 49);
	// S-box Table
    int s[8][4][16] = {{{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
						{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
						{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
						{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
					{{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
						{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
						{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
						{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
					{{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
						{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
						{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
						{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
					{{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
						{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
						{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
						{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
					{{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
						{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
						{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
						{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
					{{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
						{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
						{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
						{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
					{{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
						{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
						{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
						{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
					{{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
						{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
						{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
						{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};
	char sboxed_r[33];
	bzero(sboxed_r, 33);
	int per[32] = {16, 7, 20, 21,
					29, 12, 28, 17,
					1, 15, 23, 26,
					5, 18, 31, 10,
					2, 8, 24, 14,
					32, 27, 3, 9,
					19, 13, 30, 6,
					22, 11, 4, 25};
	//printf("%s\n", r);
	// expand r
	for (int i = 0; i < 48; i++)
	{
		expanded_r[i] = r[exp_d[i] - 1];
	}
	// xor expanded r with key
	for (int i = 0; i < 48; i++)
	{
		expanded_r[i] = ((expanded_r[i] - '0') ^ (key[i] - '0')) + '0';
	}
	// sbox
	for (int i = 0; i < 8; i++)
	{
		char row[3] = {'0', '0', '\0'};
		char col[5] = {'0', '0', '0', '0', '\0'};

		// first and last bit
		row[0] = expanded_r[i * 6 + 0];
		row[1] = expanded_r[i * 6 + 5];
		// 4 mid bits
		for (int x = 0; x < 4; x++)
			col[x] = expanded_r[i * 6 + x+1];
		// get matching s value
		int d_res = s[i][bitstoint(row)][bitstoint(col)];
		char s_res[5];
		bzero(s_res, 5);
		inttobits(d_res, s_res, 4);
		for (int j = 0; j < 4; j++)
		{
			sboxed_r[i * 4 + j] = s_res[j];
		}
	}
	// final f permute
	for (int i = 0; i < 32; i++)
	{
		res[i] = sboxed_r[per[i] - 1]; 
	}
}

void  				xor_add(char *a, char *b)
{
	for (int i = 0; i < 32; i++)
	{
		b[i] = ((b[i] - '0') ^ (a[i] - '0')) + '0';
	}
}

// ECB

void 				des_encrypt_buf(char *buf, char **res, char **keys)
{
	int initial_perm[64] = {58, 50, 42, 34, 26, 18, 10, 2,
							60, 52, 44, 36, 28, 20, 12, 4,
							62, 54, 46, 38, 30, 22, 14, 6,
							64, 56, 48, 40, 32, 24, 16, 8,
							57, 49, 41, 33, 25, 17, 9, 1,
							59, 51, 43, 35, 27, 19, 11, 3,
							61, 53, 45, 37, 29, 21, 13, 5,
							63, 55, 47, 39, 31, 23, 15, 7};
	int final_perm[64] = {40, 8, 48, 16, 56, 24, 64, 32,
							39, 7, 47, 15, 55, 23, 63, 31,
							38, 6, 46, 14, 54, 22, 62, 30,
							37, 5, 45, 13, 53, 21, 61, 29,
							36, 4, 44, 12, 52, 20, 60, 28,
							35, 3, 43, 11, 51, 19, 59, 27,
							34, 2, 42, 10, 50, 18, 58, 26,
							33, 1, 41, 9, 49, 17, 57, 25};
	char bin_buf[9 * 8];
	char ip_res[65];
	char l_block[33];
	char r_block[33];
	char Ln[33];
	char Rn[33];
	char Ln_1[33];
	char Rn_1[33];
	char Fn[65];
	char bin_res[65];

	bzero(bin_buf, 9 * 8);
	bzero(ip_res, 65);
	bzero(l_block, 33);
	bzero(r_block, 33);
	bzero(Ln, 33);
	bzero(Rn, 33);
	bzero(Ln_1, 33);
	bzero(Rn_1, 33);
	bzero(Fn, 65);
	bzero(bin_res, 65);
	// convert message to binary string
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			bin_buf[i * 8 + j] = '0' + ((buf[i] >> (7 - j)) & 1);
		}
	}
	// initial permutation
	for (int i = 0; i < 64; i++)
	{
		ip_res[i] = bin_buf[initial_perm[i] - 1];
	}
	// split in 2 halves
	ft_strncpy(l_block, ip_res, 32);
	ft_strncpy(r_block, ip_res + 32, 32);
	// 16 rounds of F
	for (int i = 0; i < 16; i++)
	{
		if (i == 0)
		{
			ft_strcpy(Ln, r_block);
			f_function(r_block, keys[i], Rn);
			//printf("%s + %s ", l_block, Rn);
			xor_add(l_block, Rn);
			//printf("= %s\n", Rn);
		}
		else
		{
			ft_strcpy(Ln, Rn_1);
			f_function(Rn_1, keys[i], Rn);
			xor_add(Ln_1, Rn);
		}
		ft_strcpy(Ln_1, Ln);
		ft_strcpy(Rn_1, Rn);
	}
	// reverse concat Rn and Ln
	ft_strncpy(Fn, Rn, 32);
	ft_strncpy(Fn + 32, Ln, 32);
	// final permutation
	for (int i = 0; i < 64; i++)
	{
		bin_res[i] = Fn[final_perm[i] - 1];
	}
	// convert binary string to char
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			(*res)[i] += (bin_res[i * 8 + j] - '0') * (1 << (7 - j));
		}
	}
}

int 				des_ecb_encrypt(char *str, size_t str_size, char **res, char **keys)
{
	int j = 0;
	char buf[9];

	if ((*res = malloc(str_size + 1)) == NULL)
		return -1;
	bzero(*res, str_size + 1);
	while (str[j])
	{
		bzero(buf, 9);
		ft_memcpy(buf, str + j, 8);
		des_encrypt_buf(buf, res, keys);
		j += 8;
	}
	return (0);
}

int 				des_ecb_decrypt(char *str, size_t str_size, char **res, char **keys)
{
	(void)str,(void)str_size,(void)res,(void)keys;
	return (0);
}

char				*des_ecb(char *str, void *data, int print)
{
	t_data *d = (t_data *)data;
	char *b64_res = NULL;
	char *des_res = NULL;
	size_t b64_res_len = 0;
	char key[56];
	char *keys[16];
	char *message = NULL;
	size_t str_size = 0;

	bzero(key, 56);
	preprocess_key(d, key);
	if (subkeys_routine(key, keys))
		return NULL;

	if (d->a_opt)
	{
		if (d->d_opt)
		{
			b64_res_len = ft_strlen(str) / 3 * 4 + 4 + 1;
			if ((b64_res = malloc(b64_res_len)) == NULL)
			{
				des_clean(keys, message);
				return NULL;
			}
			bzero(b64_res, b64_res_len);
			b64_decode_str(str, &b64_res);
			if ((message = preprocess_message(b64_res, &str_size)) == NULL)
				return NULL;
			free(b64_res);
			des_ecb_decrypt(message, str_size, &des_res, keys);
			if (print)
				printf("%s", des_res);
			des_clean(keys, message);
			return des_res;
		}
		if (d->e_opt)
		{
			if ((message = preprocess_message(str, &str_size)) == NULL)
				return NULL;
			des_ecb_encrypt(message, str_size, &des_res, keys);
			b64_res_len = ft_strlen(des_res) / 3 * 4 + 4 + 1;
			if ((b64_res = malloc(b64_res_len)) == NULL)
			{
				des_clean(keys, message);
				return NULL;
			}
			bzero(b64_res, b64_res_len);
			b64_encode_str(des_res, &b64_res);
			free(des_res);
			if (print)
				printf("%s", b64_res);
			des_clean(keys, message);
			return b64_res;
		}
	}
	else
	{
		if ((message = preprocess_message(str, &str_size)) == NULL)
			return NULL;
		if (d->d_opt)
		{
			des_ecb_decrypt(message, str_size, &des_res, keys);
		}
		else if (d->e_opt)
		{
			des_ecb_encrypt(message, str_size, &des_res, keys);
		}
	}
	if (print)
	{
		if (d->hex)
			print_hex((unsigned char *)des_res, ft_strlen(des_res));
		else
			printf("%s", des_res);
	}
	des_clean(keys, message);
	return des_res;
}

// CBC

int 				des_cbc_encrypt(char *str, char **res, char **keys)
{
	(void)str,(void)res,(void)keys;
	return (0);
}

int 				des_cbc_decrypt(char *str, char **res, char **keys)
{
	(void)str,(void)res,(void)keys;
	return (0);
}

char				*des_cbc(char *str, void *data, int print)
{
	(void)str, (void)data, (void)print;
	return NULL;
}