/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

char		*pad_len(char *str, int *padded_size)
{
	int		str_len;
	int		pad_len;
	char	*new_str;
	int		i;

	i = -1;
	pad_len = 0;
	str_len = 0;
	if (str)
		str_len = ft_strlen(str);
	while ((str_len + pad_len + 8) % 64 != 0)
		++pad_len;
	if ((new_str = (char *)malloc(str_len + pad_len)) == NULL)
		return (NULL);
	if (str)
		memmove(new_str, str, ft_strlen(str));
	new_str[str_len] = 0x80;
	i = 0;
	while (++i < pad_len)
		new_str[str_len + i] = 0;
	*padded_size = str_len + pad_len;
	return (new_str);
}

char		*add_len(char *padded_str, int *padded_size, uint64_t str_size, \
																int swap)
{
	char	*new_str;
	int		j;

	j = *padded_size;
	if ((new_str = (char *)malloc((*padded_size) + 8 + 1)) == NULL)
		return (NULL);
	ft_memmove(new_str, padded_str, *padded_size);
	free(padded_str);
	if (swap)
		str_size = ft_swap_64(str_size * 8);
	else
		str_size = str_size * 8;
	ft_memmove(new_str + j, &str_size, 8);
	*padded_size += 8;
	new_str[*padded_size] = 0;
	return (new_str);
}
