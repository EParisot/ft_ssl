/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

// Read a hex string and convert it to a char array
int 			read_hex(char *hex_str, unsigned char *out, int len)
{
	int i = 0;
	int j = 0;
	if (hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X'))
		i = 2;
	while (i < len * 2)
	{
		for (int k = 0; k < 2; k++)
		{
			if (hex_str[i + k] >= '0' && hex_str[i + k] <= '9')
			{
				if (k == 0)
					out[j] = (hex_str[i + k] - '0') * 16;
				else
					out[j] += hex_str[i + k] - '0';
			}
			else if (hex_str[i + k] >= 'a' && hex_str[i + k] <= 'f')
			{
				if (k == 0)
					out[j] = (10 + hex_str[i + k] - 'a') * 16;
				else
					out[j] += 10 + hex_str[i + k] - 'a';
			}
			else if (hex_str[i + k] >= 'A' && hex_str[i + k] <= 'F')
			{
				if (k == 0)
					out[j] = (10 + hex_str[i + k] - 'A') * 16;
				else
					out[j] += 10 + hex_str[i + k] - 'A';
			}
		}
		j++;
		i += 2;
	}
	return 0;
}

static int		read_loop(char **str, char *buf, int str_len)
{
	char		*str_tmp;

	str_tmp = NULL;
	if (*str)
	{
		if ((str_tmp = (char *)malloc(str_len + 1)) == NULL)
			return (-1);
		ft_memmove(str_tmp, *str, str_len);
		str_tmp[str_len] = 0;
		free(*str);
	}
	if (str_len)
	{
		*str = ft_strjoin(str_tmp, buf);
	}
	else
	{
		if ((*str = (char *)malloc(BUF_SIZE + 1)) == NULL)
			return (-1);
		ft_memmove(*str, buf, BUF_SIZE);
		(*str)[BUF_SIZE] = 0;
	}
	free(str_tmp);
	return (0);
}

int				read_string(t_data *data)
{
	char		buf[BUF_SIZE + 1];
	t_string	new_string;
	t_list		*new_lst;
	int  		readen = 0;

	ft_memset(buf, 0, BUF_SIZE + 1);
	if ((new_string.source = (char *)malloc(8)) == NULL)
		return (-1);
	ft_strcpy(new_string.source, "(stdin)");
	new_string.string = NULL;
	new_string.source_type = _STDIN;
	new_string.len = 0;
	while ((readen = read(STDIN_FILENO, &buf, BUF_SIZE)))
	{
		if (read_loop(&new_string.string, buf, new_string.len))
			return (-1);
		ft_memset(buf, 0, BUF_SIZE + 1);
		new_string.len += readen;
	}
	if ((new_lst = ft_lstnew(&new_string, sizeof(t_string))) == NULL)
		return (-1);
	if (data->strings == NULL)
		data->strings = new_lst;
	else
		data->strings->next = new_lst;
	return (0);
}

int				get_string(t_data *data, char *str)
{
	t_list		*tmp_lst;
	t_list		*new_lst;
	t_string	new_string;

	new_lst = NULL;
	tmp_lst = data->strings;
	while (data->strings && data->strings->next)
		data->strings = data->strings->next;
	new_string.source = NULL;
	new_string.source_type = _STRING;
	new_string.len = ft_strlen(str);
	if ((new_string.string = (char *)malloc(new_string.len + 1)) == NULL)
		return (-1);
	bzero(new_string.string, new_string.len + 1);
	ft_strcpy(new_string.string, str);
	if ((new_lst = ft_lstnew(&new_string, sizeof(t_string))) == NULL)
		return (-1);
	if (data->strings)
	{
		data->strings->next = new_lst;
		data->strings = tmp_lst;
	}
	else
		data->strings = new_lst;
	return (0);
}

static int		read_file(t_string *file)
{
	int		fd;
	char	c;
	int		len;
	int		i;

	len = 0;
	i = 0;
	if ((fd = open(file->source, O_RDONLY)) == -1)
		return (-1);
	while (read(fd, &c, 1))
		++len;
	close(fd);
	if ((file->string = (char *)malloc(len + 1)) == NULL)
		return (-1);
	if ((fd = open(file->source, O_RDONLY)) == -1)
		return (-1);
	while (read(fd, &c, 1))
	{
		(file->len)++;
		file->string[i++] = c;
	}
	file->string[i] = 0;
	close(fd);
	return (0);
}

static int		file_exists(char *filename)
{
	if (open(filename, O_RDONLY) == -1)
	{
		printf("ft_ssl: Error file %s not found...\n", filename);
		return 0;
	}
	return 1;
}

static int		already_exists(t_data *data, char *filename)
{
	t_list	*tmp_lst;

	tmp_lst = data->strings;
	if (data->strings)
	{
		while (data->strings)
		{
			if (((t_string *)(data->strings->content))->source && filename)
				if (ft_strcmp(filename, \
					((t_string *)(data->strings->content))->source) == 0)
				{
					data->strings = tmp_lst;
					return (1);
				}
			data->strings = data->strings->next;
		}
		data->strings = tmp_lst;
		while (data->strings->next)
			data->strings = data->strings->next;
	}
	return (0);
}

int				handle_files(t_data *data, char *filename)
{
	t_list		*tmp_lst;
	t_list		*new_lst;
	t_string	new_string;

	new_lst = NULL;
	tmp_lst = data->strings;
	if (!file_exists(filename) || already_exists(data, filename))
		return (-2);
	if ((new_string.source = (char *)malloc(ft_strlen(filename) + 1)) == NULL)
		return (-1);
	ft_strcpy(new_string.source, filename);
	new_string.source_type = _FILE;
	new_string.len = 0;
	if ((new_lst = ft_lstnew(&new_string, sizeof(t_string))) == NULL)
		return (-1);
	if (read_file((t_string *)(new_lst->content)))
		return (-1);
	if (data->strings)
	{
		data->strings->next = new_lst;
		data->strings = tmp_lst;
	}
	else
		data->strings = new_lst;
	return (0);
}

void 			print_res(char *res, size_t size, FILE *fp)
{
	for (size_t i = 0; i < size; i++)
	{
		if (fp == stdout)
		{
			write(1, &(res[i]), 1);
		}
		else
		{
			fprintf(fp, "%c", res[i]);
		}
	}
}