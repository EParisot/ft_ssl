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

void 				preprocess_key(t_data *data)
{
	int64_t		k = 0;

	//memcpy((char*)&k, data->key, 8);
	for (int i = 0; i < 8; ++i)
	{
		//k += ((int64_t)data->key[i]) << (i * 8);
		k += ((int64_t)data->key[i] >> 1) << (i * 7);
	}
	
	ft_memcpy(data->des_key, (char*)&k, 8);
	/*for (int i = 0; i < 8; ++i)
	{
		data->des_key[i] = (k >> (i * 8)) & 0xff;
	}*/
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
	preprocess_key(d);
	print_hex(d->des_key, 8);
	
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