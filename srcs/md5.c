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

static char	*pad_len(char *str, int *padded_size)
{
	int		str_len;
	int		pad_len;
	char	*new_str;
	int		i;

	str_len = ft_strlen(str);
	pad_len = 0;
	i = 0;
	while ((str_len + pad_len + 64) % 512 != 0)
		++pad_len;
	if ((new_str = (char *)malloc(str_len + pad_len)) == NULL)
		return (NULL);
	ft_strcpy(new_str, str);
	new_str[str_len] = 1;
	while (++i < pad_len)
		new_str[str_len + i] = 0;
	*padded_size = str_len + pad_len;
	return (new_str);
}

static char	*add_len(char *padded_str, int padded_size, long int str_size)
{
	char		*new_str;
	long int	i;
	int			j;

	i = 32;
	j = padded_size;
	if ((new_str = (char *)malloc(padded_size + 64)) == NULL)
		return (NULL);
	ft_strcpy(new_str, padded_str);
	free(padded_str);
	while (--i >= 0)
		new_str[j++] = (str_size >> i) & 1;
	i = 64;
	while (--i >= 32)
		new_str[j++] = (str_size >> i) & 1;
	return (new_str);
}

int			md5(char *str)
{
	char	*padded_str;
	int		str_size;
	int		padded_size;

	str_size = ft_strlen(str);
	padded_size = 0;
	if ((padded_str = pad_len(str, &padded_size)) == NULL)
		return (-1);
	if ((padded_str = add_len(padded_str, padded_size, str_size)) == NULL)
		return (-1);
	// TODO
	free(padded_str);
	return (0);
}
