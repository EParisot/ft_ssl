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

int    index_of(char c, char *str)
{
	for (size_t i = 0; i < ft_strlen(str); i++)
	{
		if (str[i] == c)
			return i;
	}
	return -1;
}

char *remove_char(char *str, char c)
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


int 	create_res_str(char *str, char **converted, int decode)
{
	int new_size = 0;
	
	if (decode == 0)
	{
		new_size = (ft_strlen(str) / 3 * 4);
		new_size += (ft_strlen(str) % 3) ? 4 : 0;
		new_size += new_size / 64 + 1;
	}
	else
	{
		new_size = (ft_strlen(str) / 4 * 3);
		new_size += new_size / 64 + 1;
	}
	if ((*converted = malloc(new_size + 1)) == NULL)
	{
		return -1;
	}
	ft_memset(*converted, 0, new_size + 1);
	return 0;
}

void	encode_buffer(char *buffer, int i, char **converted)
{
	uint32_t bytes_buf = (buffer[0] << 0x10) + (buffer[1] << 0x08) + buffer[2];
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
		(*converted)[out_len] = BASE64_ALPHA[bytes_buf >> (j * 6) & 0x3F];
	}
	if (buffer[1] == 0)
	{
		(*converted)[out_len - 1] = '=';
	}
	if (buffer[2] == 0)
	{
		(*converted)[out_len] = '=';
	}
}

void 	b64_encode_str(char *str, char **converted)
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
}

int		decode_buffer(char *buffer, int i, char **converted)
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

void 	b64_decode_str(char *str, char **converted)
{
	char buffer[5];

	ft_bzero(buffer, 5);
	remove_char(str, '\n');
	for (size_t i = 0; i < ft_strlen(str); i += 4)
	{
		ft_strncpy(buffer, str + i, 4);
		decode_buffer(buffer, i, converted);
	}
}

char		*base64(char *str, void *data, int print)
{
	t_data *d = (t_data *)data;
	char *converted = NULL;

	if (create_res_str(str, &converted, d->d_opt))
	{
		return NULL;
	}
	if (d->d_opt == 0)
	{
		b64_encode_str(str, &converted);
	}
	else
	{
		b64_decode_str(str, &converted);
	}
	if (d->o_opt == stdout && print)
		printf("%s", converted);
	else
	{
		if (d->d_opt == 0)
			fprintf(d->o_opt, "%s\n", converted);
		else
			fprintf(d->o_opt, "%s", converted);
		fclose(d->o_opt);
	}
	return converted;
}