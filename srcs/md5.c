/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
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
	while (i < 4)
	{
		if ((tmp_res = ft_u_itoa_base(ft_swap_32(result[i]), 16)) == NULL)
			return (-1);
		ft_memmove(str_res + tmp_size, tmp_res, 8);
		tmp_size += 8;
		free(tmp_res);
		i++;
	}
	str_res[8 * 4] = 0;
	return (0);
}

static void		rotation_md5(int i, uint32_t *word_16, uint32_t *tmp_res)
{
	uint32_t	f;
	uint32_t	tmp;
	uint32_t	fct_res;

	fct_res = 0;
	f = 0;
	if (i < 16)
		f = md5f(tmp_res[1], tmp_res[2], tmp_res[3]);
	else if (i < 32)
		f = md5g(tmp_res[1], tmp_res[2], tmp_res[3]);
	else if (i < 48)
		f = md5h(tmp_res[1], tmp_res[2], tmp_res[3]);
	else if (i < 64)
		f = md5i(tmp_res[1], tmp_res[2], tmp_res[3]);
	tmp = tmp_res[3];
	tmp_res[3] = tmp_res[2];
	tmp_res[2] = tmp_res[1];
	fct_res = (tmp_res[0] + f + word_16[md5k(i)] + md5t(i));
	tmp_res[1] += rot_l(fct_res, md5s(i));
	tmp_res[0] = tmp;
}

static void		compute_md5(uint32_t *word_16, uint32_t *result)
{
	uint32_t	tmp_res[4];
	int			i;

	i = -1;
	while (++i < 4)
		tmp_res[i] = result[i];
	i = -1;
	while (++i < 64)
		rotation_md5(i, word_16, tmp_res);
	i = -1;
	while (++i < 4)
		result[i] += tmp_res[i];
}

static int		md5_loop(unsigned char *padded_str, int padded_size, \
						char *str_res)
{
	int			i;
	uint32_t	result[4];

	i = -1;
	result[0] = 0x67452301;
	result[1] = 0xefcdab89;
	result[2] = 0x98badcfe;
	result[3] = 0x10325476;
	while (++i < (padded_size / 64))
		compute_md5((uint32_t *)(padded_str + (i * 64)), result);
	if (compute_res(result, str_res))
		return (-1);
	return (0);
}

int				md5(char *str)
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
	if ((padded_str = add_len(padded_str, &padded_size, str_size, 0)) == NULL)
		return (-1);
	if ((str_res = (char *)malloc(8 * 4 + 1)) == NULL)
		return (-1);
	if (md5_loop((unsigned char *)padded_str, padded_size, str_res))
		return (-1);
	ft_putstr(str_res);
	free(padded_str);
	free(str_res);
	return (0);
}
