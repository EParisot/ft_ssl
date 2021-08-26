/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_cbc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 17:12:42 by eparisot          #+#    #+#             */
/*   Updated: 2021/08/07 17:12:42 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/des.h"
// https://csrc.nist.gov/CSRC/media/Publications/fips/46/1/archive/1988-01-22/documents/NBS.FIPS.46-1.pdf
void 		des_cbc_buf(char *buf, char **keys, int mode)
{
	t_des 	des;
	char 	tmp[9];

	bzero(tmp, 9);
	bzero(&des, sizeof(t_des));
	// convert message to binary string
	str_to_bin((char *)buf, des.bin_buf);
	// initial permutation
	for (int i = 0; i < 64; i++)
	{
		des.ip_res[i] = des.bin_buf[initial_perm[i] - 1];
	}
	// split in 2 halves
	ft_strncpy(des.l_block, des.ip_res, 32);
	ft_strncpy(des.r_block, des.ip_res + 32, 32);
	// 16 rounds of F
	for (int i = 0; i < 16; i++)
	{
		if (i == 0)
		{
			ft_strcpy(des.Ln, des.r_block);
			f_function(des.r_block, keys[(mode == 0) ? i : 15 - i], des.Rn);
			//printf("%s + %s ", des.l_block, des.Rn);
			xor_bin(des.l_block, des.Rn);
			//printf("= %s", des.Rn);
		}
		else
		{
			ft_strcpy(des.Ln, des.Rn_1);
			f_function(des.Rn_1, keys[(mode == 0) ? i : 15 - i], des.Rn);
			xor_bin(des.Ln_1, des.Rn);
		}
		ft_strcpy(des.Ln_1, des.Ln);
		ft_strcpy(des.Rn_1, des.Rn);
	}
	// reverse concat Rn and Ln
	ft_strncpy(des.Fn, des.Rn, 32);
	ft_strncpy(des.Fn + 32, des.Ln, 32);
	// final permutation
	for (int i = 0; i < 64; i++)
	{
		des.bin_res[i] = des.Fn[final_perm[i] - 1];
	}
	// convert binary string to char
	bin_to_str(des.bin_res, tmp);
	ft_memcpy(buf, tmp, 8);
}

int 		des_cbc_loop(char *str, size_t str_size, char **res, char **keys, t_data *data)
{
	size_t 	j = 0;
	char 	buf[9];
	char 	tmp[9];
	char 	last_buf[9];

	bzero(last_buf, 9);
	bzero(tmp, 9);
	ft_memcpy(last_buf, data->iv, 8);
	if ((*res = malloc(str_size + 1)) == NULL)
		return -1;
	bzero(*res, str_size + 1);
	while (j < str_size)
	{
		bzero(buf, 9);
		ft_memcpy(buf, str + j, 8);
		if (data->d_opt == DECRYPT)
			ft_memcpy(tmp, buf, 8);
		if (data->d_opt == ENCRYPT)
			xor(last_buf, buf);
		des_cbc_buf(buf, keys, data->d_opt);
		if (data->d_opt == DECRYPT)
		{
			xor(last_buf, buf);
			ft_memcpy(last_buf, tmp, 8);
		}
		ft_memcpy((*res) + j, buf, 8);	
		if (data->d_opt == ENCRYPT)
			ft_memcpy(last_buf, buf, 8);
		j += 8;
	}
	return (0);
}

char		*des_cbc(char *str, void *data, size_t *size)
{
	t_data 	*d = (t_data *)data;
	char 	*b64_res = NULL;
	char 	*des_res = NULL;
	size_t 	b64_res_len = 0;
	char 	key[56];
	char 	*keys[16];
	char 	*message = NULL;

	bzero(key, 56);
	preprocess_key(d, key);
	if (subkeys_routine(key, keys))
		return NULL;

	if (d->a_opt)
	{
		if (d->d_opt)
		{
			b64_res_len = *size / 3 * 4 + 4 + 1;
			if ((b64_res = malloc(b64_res_len)) == NULL)
			{
				des_clean(keys, message);
				return NULL;
			}
			bzero(b64_res, b64_res_len);
			b64_decode_str(str, &b64_res);
			*size = ft_strlen(b64_res);
			if ((message = preprocess_message(b64_res, size, d->d_opt)) == NULL)
			{
				free(b64_res);
				des_clean(keys, message);
				return NULL;
			}
			free(b64_res);
			des_cbc_loop(message, *size, &des_res, keys, d);
			postprocess_message(des_res, size);
			des_clean(keys, message);
			return des_res;
		}
		else if (d->e_opt)
		{
			if ((message = preprocess_message(str, size, d->d_opt)) == NULL)
			{
				des_clean(keys, message);
				return NULL;
			}
			des_cbc_loop(message, *size, &des_res, keys, d);
			b64_res_len = *size / 3 * 4 + 4 + 1;
			if ((b64_res = malloc(b64_res_len)) == NULL)
			{
				des_clean(keys, message);
				return NULL;
			}
			bzero(b64_res, b64_res_len);
			b64_encode_str(des_res, &b64_res);
			*size = b64_res_len;
			free(des_res);
			des_clean(keys, message);
			return b64_res;
		}
	}
	else
	{
		if ((message = preprocess_message(str, size, d->d_opt)) == NULL)
		{
			des_clean(keys, message);
			return NULL;
		}
		des_cbc_loop(message, *size, &des_res, keys, d);
		if (d->d_opt)
			postprocess_message(des_res, size);
	}
	des_clean(keys, message);
	return des_res;
}