/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha512.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

static int		compute_res(uint64_t *result, char *str_res)
{
	int				i;
	unsigned char	*tmp_res;
	int				tmp_size;

	tmp_size = 0;
	tmp_res = NULL;
	i = 0;
	while (i < 8)
	{
		if ((tmp_res = ft_lu_itoa_base(result[i], 16)) == NULL)
			return (-1);
		ft_memmove(str_res + tmp_size, tmp_res, 16);
		tmp_size += 16;
		free(tmp_res);
		i++;
	}
	str_res[8 * 16] = 0;
	return (0);
}

static void		rotation_sha512(int i, uint64_t *word_80, uint64_t *tmp_res)
{
	uint64_t	t1;
	uint64_t	t2;

	t1 = tmp_res[7] + sha512_bsig1(tmp_res[4]) + \
		sha512_ch(tmp_res[4], tmp_res[5], tmp_res[6]) + sha512_k(i) + word_80[i];
	t2 = sha512_bsig0(tmp_res[0]) + \
		sha512_maj(tmp_res[0], tmp_res[1], tmp_res[2]);
	tmp_res[7] = tmp_res[6];
	tmp_res[6] = tmp_res[5];
	tmp_res[5] = tmp_res[4];
	tmp_res[4] = tmp_res[3] + t1;
	tmp_res[3] = tmp_res[2];
	tmp_res[2] = tmp_res[1];
	tmp_res[1] = tmp_res[0];
	tmp_res[0] = t1 + t2;
}

static void		compute_sha512(uint64_t *word_16, uint64_t *result)
{
	uint64_t	tmp_res[8];
	int			i;
	uint64_t	word_80[80];

	ft_bzero(word_80, 80 * sizeof(uint64_t));
	i = -1;
	while (++i < 8)
		tmp_res[i] = result[i];
	i = -1;
	while (++i < 16)
		word_80[i] = ft_swap_32(word_16[i]);
	--i;
	while (++i < 80)
		word_80[i] = sha512_ssig1(word_80[i - 2]) + word_80[i - 7] + \
					sha512_ssig0(word_80[i - 15]) + word_80[i - 16];
	i = -1;
	while (++i < 80)
		rotation_sha512(i, word_80, tmp_res);
	i = -1;
	while (++i < 8)
		result[i] += tmp_res[i];
}

static int		sha512_loop(unsigned char *padded_str, int padded_size, \
						char *str_res)
{
	uint64_t	result[8];
	int			i;

	result[0] = 0x6a09e667f3bcc908;
	result[1] = 0xbb67ae8584caa73b;
	result[2] = 0x3c6ef372fe94f82b;
	result[3] = 0xa54ff53a5f1d36f1;
	result[4] = 0x510e527fade682d1;
	result[5] = 0x9b05688c2b3e6c1f;
	result[6] = 0x1f83d9abfb41bd6b;
	result[7] = 0x5be0cd19137e2179;
	i = -1;
	while (++i < (padded_size / 80))
		compute_sha512((uint64_t *)(padded_str + (i * 80)), result);
	if (compute_res(result, str_res))
		return (-1);
	return (0);
}
#include <stdio.h>
int				sha512(char *str)
{
	char			*padded_str;
	uint64_t		str_size;
	int				padded_size;
	char			*str_res;

	str_size = 0;
	if (str)
		str_size = ft_strlen(str);
	padded_size = 0;
	if ((padded_str = pad_len_512(str, &padded_size)) == NULL)
		return (-1);
	if ((padded_str = add_len_512(padded_str, &padded_size, str_size, 1)) == NULL)
		return (-1);

	for (int j = 0; j < 64; ++j)
	{
		//printf("%c : ", padded_str[j]);
		printf("%02x", padded_str[j]);
		if (j > 0 && (j+1) % 4 == 0)
			printf(" ");
		if (j > 0 && (j+1) % 16 == 0)
			printf("\n");
	}
	printf("\n");

	if ((str_res = (char *)malloc(8 * 16 + 1)) == NULL)
		return (-1);
	if (sha512_loop((unsigned char *)padded_str, padded_size, str_res))
		return (-1);
	ft_putstr(str_res);
	free(padded_str);
	free(str_res);
	return (0);
}
