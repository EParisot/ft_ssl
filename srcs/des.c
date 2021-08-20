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

char 				*preprocess_message(char *str)
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

void  				f_function(char *r, char *key, char *res)
{
	(void)r,(void)key,(void)res;
}

void  				xor_add(char *a, char *b)
{
	(void)a, (void)b;
}

// ECB

void 				des_encrypt_buf(char *buf, char **res, char **keys)
{(void)res, (void)keys;
	int initial_perm[64] = {58, 50, 42, 34, 26, 18, 10, 2,
							60, 52, 44, 36, 28, 20, 12, 4,
							62, 54, 46, 38, 30, 22, 14, 6,
							64, 56, 48, 40, 32, 24, 16, 8,
							57, 49, 41, 33, 25, 17, 9, 1,
							59, 51, 43, 35, 27, 19, 11, 3,
							61, 53, 45, 37, 29, 21, 13, 5,
							63, 55, 47, 39, 31, 23, 15, 7};
	char bin_buf[9 * 8];
	char ip_res[65];
	char l_block[33];
	char r_block[33];
	char Ln[33];
	char Rn[33];
	char Ln_1[33];
	char Rn_1[33];

	// convert message to binary string
	bzero(bin_buf, 9 * 8);
	bzero(ip_res, 65);
	bzero(l_block, 33);
	bzero(r_block, 33);
	bzero(Ln, 33);
	bzero(Rn, 33);
	bzero(Ln_1, 33);
	bzero(Rn_1, 33);
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
			xor_add(l_block, Rn);
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
}

int 				des_ecb_encrypt(char *str, char **res, char **keys)
{
	int j = 0;
	char buf[8];

	while (str[j])
	{
		bzero(buf, 8);
		ft_memcpy(buf, str + j, 8);
		des_encrypt_buf(buf, res, keys);
		j += 8;
	}
	return (0);
}

int 				des_ecb_decrypt(char *str, char **res, char **keys)
{
	(void)str,(void)res,(void)keys;
	return (0);
}

char				*des_ecb(char *str, void *data, int print)
{
	t_data *d = (t_data *)data;
	char *b64_res = NULL;
	char *des_res = NULL;
	size_t b64_res_len = ft_strlen(str) / 3 * 4 + 4 + 1;
	char key[56];
	char *keys[16];
	char *message = NULL;

	bzero(key, 56);
	preprocess_key(d, key);
	if (subkeys_routine(key, keys))
		return NULL;

	if (d->a_opt)
	{
		if (d->d_opt)
		{
			if ((b64_res = malloc(b64_res_len)) == NULL)
			{
				des_clean(keys, message);
				return NULL;
			}
			bzero(b64_res, b64_res_len);
			b64_decode_str(str, &b64_res);
			if ((message = preprocess_message(b64_res)) == NULL)
				return NULL;
			des_ecb_decrypt(message, &des_res, keys);
			if (print)
				printf("%s", des_res);
			des_clean(keys, message);
			return des_res;
		}
		if (d->e_opt)
		{
			if ((b64_res = malloc(b64_res_len)) == NULL)
			{
				des_clean(keys, message);
				return NULL;
			}
			bzero(b64_res, b64_res_len);
			if ((message = preprocess_message(str)) == NULL)
				return NULL;
			des_ecb_encrypt(message, &des_res, keys);
			b64_encode_str(des_res, &b64_res);
			if (print)
				printf("%s", b64_res);
			des_clean(keys, message);
			return b64_res;
		}
	}
	else
	{
		if ((message = preprocess_message(str)) == NULL)
			return NULL;
		if (d->d_opt)
		{
			des_ecb_decrypt(message, &des_res, keys);
		}
		else if (d->e_opt)
		{
			des_ecb_encrypt(message, &des_res, keys);
		}
	}
	if (print)
		printf("%s", des_res);
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
	t_data *d = (t_data *)data;
	char *b64_res = NULL;
	char *des_res = NULL;
	size_t b64_res_len = ft_strlen(str) / 3 * 4 + 4 + 1;
	char key[56];
	char *keys[16];
	char *message = NULL;

	bzero(key, 56);
	preprocess_key(d, key);
	if (subkeys_routine(key, keys))
		return NULL;
	if ((message = preprocess_message(str)) == NULL)
		return NULL;

	if (d->a_opt)
	{
		if (d->d_opt)
		{
			if ((b64_res = malloc(b64_res_len)) == NULL)
			{
				des_clean(keys, message);
				return NULL;
			}
			bzero(b64_res, b64_res_len);
			b64_decode_str(str, &b64_res);
			des_cbc_decrypt(b64_res, &des_res, keys);
			if (print)
				printf("%s", des_res);
			des_clean(keys, message);
			return des_res;
		}
		if (d->e_opt)
		{
			if ((b64_res = malloc(b64_res_len)) == NULL)
			{
				des_clean(keys, message);
				return NULL;
			}
			bzero(b64_res, b64_res_len);
			des_cbc_encrypt(str, &des_res, keys);
			b64_encode_str(des_res, &b64_res);
			if (print)
				printf("%s", b64_res);
			des_clean(keys, message);
			return b64_res;
		}
	}
	else
	{
		if (d->d_opt)
		{
			des_cbc_decrypt(str, &des_res, keys);
		}
		else if (d->e_opt)
		{
			des_cbc_encrypt(str, &des_res, keys);
		}
	}
	if (print)
		printf("%s", des_res);
	des_clean(keys, message);
	return des_res;
}