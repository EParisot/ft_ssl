/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

static int		compute_res(uint32_t *result, char *str_res)
{
	int				i;
	unsigned char	*tmp_res;
	int				tmp_size;

	tmp_size = 0;
	tmp_res = NULL;
	i = 0;
	while (i < 8)
	{
		if ((tmp_res = ft_u_itoa_base(ft_swap_32(result[i]), 16)) == NULL)
			return (-1);
		ft_memmove(str_res + tmp_size, tmp_res, 8);
		tmp_size += 8;
		free(tmp_res);
		i++;
	}
	str_res[8 * 8] = 0;
	return (0);
}

static int		sha256_loop(unsigned char *padded_str, int padded_size, \
						char *str_res)
{
	(void)padded_size;
	(void)padded_str;
	uint32_t	result[8];

	result[0] = 0x6a09e667;
    result[1] = 0xbb67ae85;
    result[2] = 0x3c6ef372;
    result[3] = 0xa54ff53a;
    result[4] = 0x510e527f;
    result[5] = 0x9b05688c;
    result[6] = 0x1f83d9ab;
    result[7] = 0x5be0cd19;
	// TODO
	if (compute_res(result, str_res))
		return (-1);
	return (0);
}

int			sha256(char *str)
{
	char			*padded_str;
	uint64_t		str_size;
	int				padded_size;
	char			*str_res;

	str_size = 0;
	if (str)
		str_size = ft_strlen(str);
	padded_size = 0;
	if ((padded_str = pad_len(str, &padded_size)) == NULL)
		return (-1);
	if ((padded_str = add_len(padded_str, &padded_size, str_size)) == NULL)
		return (-1);
	if ((str_res = (char *)malloc(8 * 8 + 1)) == NULL)
		return (-1);
	if (sha256_loop((unsigned char *)padded_str, padded_size, str_res))
		return (-1);
	ft_putstr(str_res);
	free(padded_str);
	free(str_res);
	return (0);
}
