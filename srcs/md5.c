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

static char		*pad_len(char *str, int *padded_size)
{
	int		str_len;
	int		pad_len;
	char	*new_str;
	int		i;

	str_len = 0;
	if (str)
		str_len = ft_strlen(str);
	pad_len = 0;
	i = 0;
	while ((str_len + pad_len + 8) % 64 != 0)
		++pad_len;
	if ((new_str = (char *)malloc(str_len + pad_len)) == NULL)
		return (NULL);
	if (str)
		ft_strcpy(new_str, str);
	new_str[str_len] = 1 << 7;
	while (++i < pad_len)
		new_str[str_len + i] = 0;
	*padded_size = str_len + pad_len;
	return (new_str);
}

static char		*add_len(char *padded_str, int *padded_size, unsigned int str_size)
{
	char		*new_str;
	int			j;

	j = *padded_size;
	if ((new_str = (char *)malloc(*padded_size + 8)) == NULL)
		return (NULL);
	ft_memmove(new_str, padded_str, *padded_size);
	free(padded_str);
	str_size *= 8;
	ft_memmove(&new_str[j], &str_size, 8);
	*padded_size += 8;
	return (new_str);
}

static int		compute_res(uint32_t *result, char *str_res)
{
	int			i;
	char		*tmp_res;
	int			tmp_size;

	tmp_size = 0;
	tmp_res = NULL;
	i = 0;
	while (i < 4)
	{
		if ((tmp_res = ft_u_itoa_base(ft_swap_32(result[i]), 16)) == NULL)
			return (-1);
		ft_memmove(&str_res[tmp_size], tmp_res, 8 + 1);
		tmp_size += ft_strlen(tmp_res);
		free(tmp_res);
		i++;
	}
	str_res[32 * 4] = 0;
	return (0);
}

static void		rotation_md5(int i, char *word, uint32_t *tmp_res)
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
	fct_res = (tmp_res[0] + f + word[md5kts(i, 'k')] + md5kts(i, 't'));
	if (fct_res << md5kts(i, 's'))
		tmp_res[1] += fct_res << md5kts(i, 's');
	else
		tmp_res[1] += fct_res >> (32 - md5kts(i, 's'));
	tmp_res[0] = tmp;
}

static void		compute_md5(char *word_16, uint32_t *result)
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

static int		md5_loop(char *padded_str, int padded_size, char *str_res)
{
	int			i;
	uint32_t	result[4];

	i = -1;
	result[0] = 0x67452301;
	result[1] = 0xefcdab89;
	result[2] = 0x98badcfe;
	result[3] = 0x10325476;
	while (++i < (padded_size / 64))
		compute_md5(&padded_str[i * 64], result);
	if (compute_res(result, str_res))
		return (-1);
	return (0);
}

int				md5(char *str)
{
	char			*padded_str;
	unsigned int	str_size;
	int				padded_size;
	char			*res_str;

	str_size = 0;
	if (str)
		str_size = ft_strlen(str);
	padded_size = 0;
	if ((padded_str = pad_len(str, &padded_size)) == NULL)
		return (-1);
	if ((padded_str = add_len(padded_str, &padded_size, str_size)) == NULL)
		return (-1);
	if ((res_str = (char *)malloc(8 * 4 + 1)) == NULL)
		return (-1);
	if (md5_loop(padded_str, padded_size, res_str))
		return (-1);
	ft_putstr(res_str);
	free(padded_str);
	free(res_str);
	return (0);
}
