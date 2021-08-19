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

//https://www.geeksforgeeks.org/data-encryption-standard-des-set-1/
//http://page.math.tu-berlin.de/~kant/teaching/hess/krypto-ws2006/des.htm

void 				preprocess_key(t_data *data)
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
	char p[57];
	int64_t total = 0;
	char *start = &p[0];

	bzero(k, 65);
	bzero(p, 57);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			k[i * 8 + j] = '0' + ((data->key[i] >> (7 - j)) & 1);
		}
	}
	for (int i = 0; i < 56; i++)
	{
		p[i] = k[kperm[i] - 1];
	}
	while (*start)
	{
		total *= 2;
		if (*start++ == '1') total += 1;
	}
	for (int i = 0; i < 7; i++)
	{
		data->des_key[i] = (total >> ((6 - i) * 8)) & 0xFF;
	}
}

void 				preprocess_key_alt(t_data *data)
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
	int64_t		k = 0;
	int64_t		p = 0;

	for (int i = 0; i < 8; i++)
	{
		k += (int64_t)data->key[i] << ((7 - i) * 8);
	}
	for (int i = 0; i < 56; i++)
	{
		p += ((k >> (64 - kperm[i])) & 1) << (55 - i);
	}
	for (int i = 0; i < 7; i++)
	{
		data->des_key[i] = (p >> ((6 - i) * 8)) & 0xFF;
	}
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

char				*des_ecb(char *str, void *data, int print)
{
	t_data *d = (t_data *)data;
	char *b64_res = NULL;
	char *des_res = NULL;
	size_t b64_res_len = ft_strlen(str) / 3 * 4 + 4 + 1;
	preprocess_key_alt(d);
	
	if (d->a_opt)
	{
		if (d->d_opt)
		{
			if ((b64_res = malloc(b64_res_len)) == NULL)
				return NULL;
			bzero(b64_res, b64_res_len);
			b64_decode_str(str, &b64_res);
			des_ecb_decrypt(b64_res, &des_res);
			if (print)
				printf("%s", des_res);
			return des_res;
		}
		if (d->e_opt)
		{
			if ((b64_res = malloc(b64_res_len)) == NULL)
				return NULL;
			bzero(b64_res, b64_res_len);
			des_ecb_encrypt(str, &des_res);
			b64_encode_str(des_res, &b64_res);
			if (print)
				printf("%s", b64_res);
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
	return des_res;
}