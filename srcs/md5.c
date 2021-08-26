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

#include "../includes/ft_ssl.h"

uint32_t			md5_f(uint32_t x, uint32_t y, uint32_t z)
{
	return ((x & y) | ((~x) & z));
}

uint32_t			md5_g(uint32_t x, uint32_t y, uint32_t z)
{
	return ((x & z) | (y & (~z)));
}

uint32_t			md5_h(uint32_t x, uint32_t y, uint32_t z)
{
	return (x ^ y ^ z);
}

uint32_t			md5_i(uint32_t x, uint32_t y, uint32_t z)
{
	return (y ^ (x | (~z)));
}

uint32_t			md5_k(int i)
{
	static uint32_t k[64] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
		1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12,
		5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2,
		0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9
	};

	return (k[i]);
}

uint32_t			md5_t(int i)
{
	static uint32_t	t[64] = {
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a,
		0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340,
		0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
		0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa,
		0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
		0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
	};

	return (t[i]);
}

uint32_t			md5_s(int i)
{
	static uint32_t	s[64] = {
		7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
		5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
		4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
		6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
	};

	return (s[i]);
}

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
		f = md5_f(tmp_res[1], tmp_res[2], tmp_res[3]);
	else if (i < 32)
		f = md5_g(tmp_res[1], tmp_res[2], tmp_res[3]);
	else if (i < 48)
		f = md5_h(tmp_res[1], tmp_res[2], tmp_res[3]);
	else if (i < 64)
		f = md5_i(tmp_res[1], tmp_res[2], tmp_res[3]);
	tmp = tmp_res[3];
	tmp_res[3] = tmp_res[2];
	tmp_res[2] = tmp_res[1];
	fct_res = (tmp_res[0] + f + word_16[md5_k(i)] + md5_t(i));
	tmp_res[1] += rot_l(fct_res, md5_s(i));
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

char				*md5(char *str, void *data)
{
	(void)data;
	char			*padded_str;
	uint64_t		str_size;
	int				padded_size;
	char			*str_res;

	str_size = 0;
	if (str)
		str_size = ft_strlen(str);
	padded_size = 0;
	if ((padded_str = pad_len(str, &padded_size)) == NULL)
		return (NULL);
	if ((padded_str = add_len(padded_str, &padded_size, str_size, 0)) == NULL)
		return (NULL);
	if ((str_res = (char *)malloc(8 * 4 + 1)) == NULL)
		return (NULL);
	if (md5_loop((unsigned char *)padded_str, padded_size, str_res))
		return (NULL);
	free(padded_str);
	return str_res;
}
