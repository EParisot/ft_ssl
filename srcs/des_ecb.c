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
							21, 13, 5, 28, 20, 12, 4
							};
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

int 				subkeys_routine(char *key, char ***keys)
{
	char l_key[29]; 
	char r_key[29];

	bzero(l_key, 29);
	bzero(r_key, 29);
	ft_strncpy(l_key, key, 28);
	ft_strncpy(r_key, key + 28, 28);
	//printf("\niter 00: %s - %s\n", l_key, r_key);
	for (int i = 0; i < 16; i++)
	{
		if ((keys[0][i] = malloc(29)) == NULL)
			return -1;
		if ((keys[1][i] = malloc(29)) == NULL)
			return -1;
		bzero(keys[0][i], 29);
		bzero(keys[1][i], 29);
		if (i == 0)
		{
			ft_strcpy(keys[0][i], rot_str_l(l_key, 1));
			ft_strcpy(keys[1][i], rot_str_l(r_key, 1));
		}
		else if (i == 1 || i == 8 || i == 15)
		{
			ft_strcpy(keys[0][i], rot_str_l(keys[0][i-1], 1));
			ft_strcpy(keys[1][i], rot_str_l(keys[1][i-1], 1));
		}
		else
		{
			ft_strcpy(keys[0][i], rot_str_l(keys[0][i-1], 2));
			ft_strcpy(keys[1][i], rot_str_l(keys[1][i-1], 2));
		}
		//printf("iter %02d: %s - %s\n", i+1, keys[0][i], keys[1][i]);
	}
	return 0;
}

int 				des_ecb_encrypt(char *str, char **res)
{
	(void)str,(void)res;
	return (0);
}

int 				des_ecb_decrypt(char *str, char **res)
{
	(void)str,(void)res;
	return (0);
}

void 				des_clean(char ***keys)
{
	for (int i = 0; i < 16; i++)
	{
		free(keys[0][i]);
		free(keys[1][i]);
	}
}

char				*des_ecb(char *str, void *data, int print)
{
	t_data *d = (t_data *)data;
	char *b64_res = NULL;
	char *des_res = NULL;
	size_t b64_res_len = ft_strlen(str) / 3 * 4 + 4 + 1;
	
	char key[56];
	char *l_keys[16];
	char *r_keys[16];
	char **keys[2] = {
		l_keys,
		r_keys
	};

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
			des_ecb_decrypt(b64_res, &des_res);
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
			des_ecb_encrypt(str, &des_res);
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
			des_ecb_decrypt(str, &des_res);
		}
		else if (d->e_opt)
		{
			des_ecb_encrypt(str, &des_res);
		}
	}
	if (print)
		printf("%s", des_res);
	des_clean(keys);
	return des_res;
}