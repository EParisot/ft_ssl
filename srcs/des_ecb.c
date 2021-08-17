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

int 				des_ecb_encrypt()
{

	return (0);
}

int 				des_ecb_decrypt()
{

	return (0);
}

int 				des_ecb(char *str, void *data)
{
	t_data *d = (t_data *)data;
	char *b64_res = NULL;
	char *des_res = NULL;
	size_t b64_res_len = ft_strlen(str) / 3 * 4 + 4 + 1;
	
	if (d->a_opt)
	{
		if (d->d_opt)
		{
			if ((b64_res = malloc(b64_res_len)) == NULL)
				return -1;
			bzero(b64_res, b64_res_len);
			b64_decode_str(str, &b64_res);
			// TODO DES + print
			free(b64_res);
			return 0;
		}
		if (d->e_opt)
		{
			if ((b64_res = malloc(b64_res_len)) == NULL)
				return -1;
			bzero(b64_res, b64_res_len);
			// TODO DES
			b64_encode_str(des_res, &b64_res);
			// TODO print b64_res
			free(b64_res);
		}
	}
	else
	{
		if (d->d_opt)
		{
			// TODO DES
		}
		else if (d->e_opt)
		{
			// TODO DES
		}
	}
	return 0;
}