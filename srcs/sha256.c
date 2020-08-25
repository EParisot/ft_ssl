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

static int		sha256_loop(unsigned char *padded_str, int padded_size, \
						char *str_res)
{
	(void)padded_size;
	(void)padded_str;
	(void)str_res;
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
	if ((str_res = (char *)malloc(8 * 4 + 1)) == NULL)
		return (-1);
	if (sha256_loop((unsigned char *)padded_str, padded_size, str_res))
		return (-1);
	ft_putstr(str_res);
	free(padded_str);
	free(str_res);
	return (0);
}
