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

#include "../includes/ft_ssl.h"

static int index_of(char c, char *str)
{
	for (size_t i = 0; i < ft_strlen(str); i++)
	{
		if (str[i] == c)
			return i;
	}
	return -1;
}

static char *remove_char(char *str, char c)
{
	for (size_t i = 0; i < ft_strlen(str); i++)
	{
		if (str[i] == c)
		{
			size_t j = i;
			for (j = i; j < ft_strlen(str); j++)
			{
				str[j] = str[j + 1];
			}
			str[j] = '\0';
			i--;
		}
	}
	return str;
}


static int create_res_str(char *str, char **converted, int decode, size_t *size)
{
	int new_size = 0;
	
	if (decode == 0)
	{
		new_size = (*size / 3) * 4;
		new_size += (*size % 3) ? 4 : 0;
		if (new_size >= 64)
			new_size += new_size / 64 + 1;
	}
	else
	{
		new_size = (((*size) - char_count(str, '\n')) / 4) * 3;
		new_size -= char_count(str, '=');
	}
	if ((*converted = malloc(new_size + 1)) == NULL)
	{
		return -1;
	}
	ft_memset(*converted, 0, new_size + 1);
	return new_size;
}

static void	encode_buffer(char *buffer, int i, char **converted, int size)
{
	uint32_t bytes_buf = (((uint8_t)buffer[0]) << 0x10) + (((uint8_t)buffer[1]) << 0x08) + (uint8_t)buffer[2];
	int in_len = 0;
	int out_len = 0;

	for (int j = 3; j >= 0; j--)
	{
		out_len = ft_strlen(*converted);
		in_len = i / 3 * 4 + (3-j);
		if (in_len && in_len % 64 == 0)
		{
			(*converted)[out_len] = '\n';
			out_len += 1;
		}
		(*converted)[out_len] = BASE64_ALPHA[(bytes_buf >> (j * 6)) & 0x3F];
	}
	if (i + 1 >= size)
	{
		(*converted)[out_len - 1] = '=';
	}
	if (i + 2 >= size)
	{
		(*converted)[out_len] = '=';
	}
}

static void b64_encode_str(char *str, char **converted, size_t *size)
{
	char buffer[4];

	ft_bzero(buffer, 4);
	for (size_t i = 0; i < *size; i += 3)
	{
		buffer[0] = str[i];
		if (i + 1 < *size)
			buffer[1] = str[i + 1];
		else
			buffer[1] = '\0';
		if (i + 2 < *size)
			buffer[2] = str[i + 2];
		else
			buffer[2] = '\0';
		encode_buffer(buffer, i, converted, *size);
	}
}

static int decode_buffer(char *buffer, int i, char **converted)
{
	uint32_t bytes_buf = 0;

	if (buffer[2] == '=')
	{
		buffer[2] = 0;
	}
	if (buffer[3] == '=')
	{
		buffer[3] = 0;
	}
	for (int j = 0; j < 4; j++)
	{
		int idx = 0;
		if ((idx = index_of(buffer[j], BASE64_ALPHA)) == -1)
		    continue;
		bytes_buf += idx << ((3-j) * 6);
	}
	for (int j = 0; j < 3; j++)
	{
		int idx = i / 4 * 3 + j;
		int val = 0;
		if ((val = bytes_buf >> ((2-j) * 8) & 0xFF))
			(*converted)[idx] = val;
	}
	return 0;
}

static void	b64_decode_str(char *str, char **converted, size_t *size)
{
	char buffer[5];

	ft_bzero(buffer, 5);
	remove_char(str, '\n');
	for (size_t i = 0; i < *size; i += 4)
	{
		ft_strncpy(buffer, str + i, 4);
		decode_buffer(buffer, i, converted);
	}
}

char		*base64(char *str, void *data, size_t *size)
{
	t_data *d = (t_data *)data;
	char *converted = NULL;
	int new_size = 0;

	if ((new_size = create_res_str(str, &converted, d->d_opt, size)) == -1)
	{
		return NULL;
	}
	if (d->d_opt == 0)
	{
		b64_encode_str(str, &converted, size);
		if (new_size >= 64 && converted[new_size - 1] == '\0')
			converted[new_size - 1] = '\n';
	}
	else
	{
		b64_decode_str(str, &converted, size);
	}
	*size = new_size;
	return converted;
}