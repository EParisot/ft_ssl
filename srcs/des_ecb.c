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

	bzero(k, 65);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			k[i * 8 + j] = '0' + ((data->key[i] >> (7 - j)) & 1);
		}
	}
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

	bzero(l_key, 29);
	bzero(r_key, 29);
	ft_strncpy(l_key, key, 28);
	ft_strncpy(r_key, key + 28, 28);

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

		if ((concat_keys[i] = malloc(57)) == NULL)
			return -1;
		bzero(concat_keys[i], 57);
		ft_strncpy(concat_keys[i], key_pairs[0][i], 28);
		ft_strncat(concat_keys[i], key_pairs[1][i], 28);

		if ((keys[i] = malloc(49)) == NULL)
			return -1;
		bzero(keys[i], 49);
		for (int j = 0; j < 48; j++)
		{
			keys[i][j] = concat_keys[i][key_comp[j] - 1];
		}
		//printf("%02d: %s\n", i, keys[i]);
	}
	for (int i = 0; i < 16; i++)
	{
		free(key_pairs[0][i]);
		free(key_pairs[1][i]);
		free(concat_keys[i]);
	}
	return 0;
}

int 				des_ecb_encrypt(char *str, char **res, char **keys)
{
	(void)str,(void)res,(void)keys;
	return (0);
}

int 				des_ecb_decrypt(char *str, char **res, char **keys)
{
	(void)str,(void)res,(void)keys;
	return (0);
}

void 				des_clean(char **keys)
{
	for (int i = 0; i < 16; i++)
	{
		free(keys[i]);
	}
}

char				*des_ecb(char *str, void *data, int print)
{
	t_data *d = (t_data *)data;
	char *b64_res = NULL;
	char *des_res = NULL;
	size_t b64_res_len = ft_strlen(str) / 3 * 4 + 4 + 1;
	char key[56];
	char *keys[16];

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
				des_clean(keys);
				return NULL;
			}
			bzero(b64_res, b64_res_len);
			b64_decode_str(str, &b64_res);
			des_ecb_decrypt(b64_res, &des_res, keys);
			if (print)
				printf("%s", des_res);
			des_clean(keys);
			return des_res;
		}
		if (d->e_opt)
		{
			if ((b64_res = malloc(b64_res_len)) == NULL)
			{
				des_clean(keys);
				return NULL;
			}
			bzero(b64_res, b64_res_len);
			des_ecb_encrypt(str, &des_res, keys);
			b64_encode_str(des_res, &b64_res);
			if (print)
				printf("%s", b64_res);
			des_clean(keys);
			return b64_res;
		}
	}
	else
	{
		if (d->d_opt)
		{
			des_ecb_decrypt(str, &des_res, keys);
		}
		else if (d->e_opt)
		{
			des_ecb_encrypt(str, &des_res, keys);
		}
	}
	if (print)
		printf("%s", des_res);
	des_clean(keys);
	return des_res;
}