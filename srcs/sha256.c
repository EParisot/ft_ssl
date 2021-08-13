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

#include "../includes/ft_ssl.h"

uint32_t	sha256_ch(uint32_t x, uint32_t y, uint32_t z)
{
	return (x & y) ^ ((~x) & z);
}

uint32_t	sha256_maj(uint32_t x, uint32_t y, uint32_t z)
{
	return (x & y) ^ (x & z) ^ (y & z);
}

uint32_t	sha256_k(int i)
{
	static uint32_t k[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
		0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
		0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
		0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
		0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
		0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
		0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
		0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
		0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};

	return (k[i]);
}

uint32_t	sha256_bsig0(uint32_t x)
{
	return (rot_r(x, 2) ^ rot_r(x, 13) ^ rot_r(x, 22));
}

uint32_t	sha256_bsig1(uint32_t x)
{
	return (rot_r(x, 6) ^ rot_r(x, 11) ^ rot_r(x, 25));
}

uint32_t	sha256_ssig0(uint32_t x)
{
	return (rot_r(x, 7) ^ rot_r(x, 18) ^ (x >> 3));
}

uint32_t	sha256_ssig1(uint32_t x)
{
	return (rot_r(x, 17) ^ rot_r(x, 19) ^ (x >> 10));
}

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
		if ((tmp_res = ft_u_itoa_base(result[i], 16)) == NULL)
			return (-1);
		ft_memmove(str_res + tmp_size, tmp_res, 8);
		tmp_size += 8;
		free(tmp_res);
		i++;
	}
	str_res[8 * 8] = 0;
	return (0);
}

static void		rotation_sha256(int i, uint32_t *word_64, uint32_t *tmp_res)
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

static void		compute_sha256(uint32_t *word_16, uint32_t *result)
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
		rotation_sha256(i, word_64, tmp_res);
	i = -1;
	while (++i < 8)
		result[i] += tmp_res[i];
}

static int		sha256_loop(unsigned char *padded_str, int padded_size, \
						char *str_res)
{
	uint32_t	result[8];
	int			i;

	result[0] = 0x6a09e667;
	result[1] = 0xbb67ae85;
	result[2] = 0x3c6ef372;
	result[3] = 0xa54ff53a;
	result[4] = 0x510e527f;
	result[5] = 0x9b05688c;
	result[6] = 0x1f83d9ab;
	result[7] = 0x5be0cd19;
	i = -1;
	while (++i < (padded_size / 64))
		compute_sha256((uint32_t *)(padded_str + (i * 64)), result);
	if (compute_res(result, str_res))
		return (-1);
	return (0);
}

int				sha256(char *str, int decode, FILE *fd)
{
	(void)fd;
	(void)decode;
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
	if ((str_res = (char *)malloc(8 * 8 + 1)) == NULL)
		return (-1);
	if (sha256_loop((unsigned char *)padded_str, padded_size, str_res))
		return (-1);
	printf("%s\n", str_res);
	free(padded_str);
	free(str_res);
	return (0);
}
