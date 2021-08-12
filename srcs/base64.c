/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 17:12:42 by eparisot          #+#    #+#             */
/*   Updated: 2021/08/07 17:12:42 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

int 	create_res_str(char *str, char **converted)
{
	int new_size = 0;

	new_size = (ft_strlen(str) / 3 * 4);
	new_size += (ft_strlen(str) % 3) ? 4 : 0;
	if ((*converted = malloc(new_size + 1)) == NULL)
	{
		return -1;
	}
	ft_memset(*converted, 0, new_size + 1);
	return 0;
}

int		encode_buffer(char *buffer, int i, char **converted)
{
	uint32_t bytes_buf = (buffer[0] << 0x10) + (buffer[1] << 0x08) + buffer[2];

	for (int j = 3; j >= 0; j--)
	{
		(*converted)[i / 3 * 4 + (3-j)] = BASE64_ALPHA[bytes_buf >> (j * 6) & 0x3F];
	}
	if (buffer[1] == 0)
	{
		(*converted)[i / 3 * 4 + 2] = '=';
	}
	if (buffer[2] == 0)
	{
		(*converted)[i / 3 * 4 + 3] = '=';
	}
	return 0;
}

int 	encode_str(char *str, char **converted)
{
	char buffer[4];

	ft_bzero(buffer, 4);
	for (size_t i = 0; i < ft_strlen(str); i += 3)
	{
		buffer[0] = str[i];
		if (i + 1 < ft_strlen(str))
			buffer[1] = str[i + 1];
		else
			buffer[1] = '\0';
		if (i + 2 < ft_strlen(str))
			buffer[2] = str[i + 2];
		else
			buffer[2] = '\0';
		encode_buffer(buffer, i, converted);
	}
	return 0;
}

int		base64(char *str)
{
	char *converted = NULL;

	if (create_res_str(str, &converted))
	{
		return 1;
	}
	if (encode_str(str, &converted))
	{
		return 1;
	}
	printf("%s\n", converted);
	free(converted);
	return 0;
}