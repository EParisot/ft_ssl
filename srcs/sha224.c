/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha224.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

static int		compute_res(uint32_t *result, char *str_res)
{
	int				i;
	unsigned char	*tmp_res;
	int				tmp_size;

	tmp_size = 0;
	tmp_res = NULL;
	i = 0;
	while (i < 7)
	{
		if ((tmp_res = ft_u_itoa_base(result[i], 16)) == NULL)
			return (-1);
		ft_memmove(str_res + tmp_size, tmp_res, 8);
		tmp_size += 8;
		free(tmp_res);
		i++;
	}
	str_res[7 * 8] = 0;
	return (0);
}

static void		rotation_sha224(int i, uint32_t *word_64, uint32_t *tmp_res)
{
	uint32_t	t1;
	uint32_t	t2;

	t1 = tmp_res[7] + sha256_bsig1(tmp_res[4]) + \
		sha256_ch(tmp_res[4], tmp_res[5], tmp_res[6]) + sha256_k(i) \
														+ word_64[i];
	t2 = sha256_bsig0(tmp_res[0]) + \
		sha256_maj(tmp_res[0], tmp_res[1], tmp_res[2]);
	tmp_res[7] = tmp_res[6];
	tmp_res[6] = tmp_res[5];
	tmp_res[5] = tmp_res[4];
	tmp_res[4] = tmp_res[3] + t1;
	tmp_res[3] = tmp_res[2];
	tmp_res[2] = tmp_res[1];
	tmp_res[1] = tmp_res[0];
	tmp_res[0] = t1 + t2;
}

static void		compute_sha224(uint32_t *word_16, uint32_t *result)
{
	uint32_t	tmp_res[8];
	int			i;
	uint32_t	word_64[64];

	ft_bzero(word_64, 64 * sizeof(uint32_t));
	i = -1;
	while (++i < 8)
		tmp_res[i] = result[i];
	i = -1;
	while (++i < 16)
		word_64[i] = ft_swap_32(word_16[i]);
	--i;
	while (++i < 64)
		word_64[i] = sha256_ssig1(word_64[i - 2]) + word_64[i - 7] + \
					sha256_ssig0(word_64[i - 15]) + word_64[i - 16];
	i = -1;
	while (++i < 64)
		rotation_sha224(i, word_64, tmp_res);
	i = -1;
	while (++i < 8)
		result[i] += tmp_res[i];
}

static int		sha224_loop(unsigned char *padded_str, int padded_size, \
						char *str_res)
{
	uint32_t	result[8];
	int			i;

	result[0] = 0xc1059ed8;
	result[1] = 0x367cd507;
	result[2] = 0x3070dd17;
	result[3] = 0xf70e5939;
	result[4] = 0xffc00b31;
	result[5] = 0x68581511;
	result[6] = 0x64f98fa7;
	result[7] = 0xbefa4fa4;
	i = -1;
	while (++i < (padded_size / 64))
		compute_sha224((uint32_t *)(padded_str + (i * 64)), result);
	if (compute_res(result, str_res))
		return (-1);
	return (0);
}

int				sha224(char *str, void *data)
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
		return (-1);
	if ((padded_str = add_len(padded_str, &padded_size, str_size, 1)) == NULL)
		return (-1);
	if ((str_res = (char *)malloc(7 * 8 + 1)) == NULL)
		return (-1);
	if (sha224_loop((unsigned char *)padded_str, padded_size, str_res))
		return (-1);
	printf("%s\n", str_res);
	free(padded_str);
	free(str_res);
	return (0);
}
