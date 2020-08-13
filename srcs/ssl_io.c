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

#include "../includes/ft_ssl_md5.h"

int			read_loop(t_data *data, char *buf)
{
	static int	i;
	char	*str_tmp;
	int		len;

	len = 0;
	str_tmp = NULL;
	if (data->string)
	{
		len = ft_strlen(data->string);
		if ((str_tmp = (char *)malloc(len + 1)) == NULL)
			return (-1);
		ft_memmove(str_tmp, data->string, len);
		free(data->string);
	}
	if ((data->string = (char *)malloc(BUF_SIZE * ++i + 1)) == NULL)
		return (-1);
	if (len)
	{
		ft_memmove(data->string, str_tmp, len);
		ft_strcat(data->string, buf);
	}
	else
		ft_memmove(data->string, buf, BUF_SIZE);
	free(str_tmp);
	return (0);
}

int			read_stdin(t_data *data)
{
	char 	buf[BUF_SIZE];

	ft_memset(buf, 0, BUF_SIZE);
	while(read(STDIN_FILENO, &buf, BUF_SIZE))
	{
		if (read_loop(data, buf))
			return (-1);
		ft_memset(buf, 0, BUF_SIZE);
	}
	ft_putchar('\n');
	return (0);
}

int			read_file(t_data *data, char *filename)
{
	int 	fd;
	char 	c;
	int		len;
	int		i;

	len = 0;
	i = 0;
	if ((fd = open(filename, O_RDONLY)) == -1)
		return (-1);
	while (read(fd, &c, 1))
		++len;
	close(fd);
	if ((data->string = (char *)malloc(len + 1)) == NULL)
		return (-1);
	if ((fd = open(filename, O_RDONLY)) == -1)
		return (-1);
	while (read(fd, &c, 1))
		data->string[i++] = c;
	data->string[i] = 0;
	close(fd);
	return (0);
}

int			read_files(t_data *data)
{
	t_list	*tmp_lst;

	tmp_lst = data->files;
	while (data->files)
	{
		if (read_file(data, data->files->content))
		{
			data->files = tmp_lst;
			return (-1);
		}
		if (hash_string(data))
			return (-1);
		if (data->string)
		{
			free(data->string);
			data->string = NULL;
		}
		data->files = data->files->next;
	}
	data->files = tmp_lst;
	return (0);
}

int 		handle_files(t_data *data, char *filename)
{
	t_list	*tmp_lst;
	t_list	*new_lst;

	new_lst = NULL;
	tmp_lst = NULL;
	if (data->files)
	{
		tmp_lst = data->files;
		while (data->files->next)
		{
			if (ft_strcmp(filename, data->files->content) == 0)
				return (-2);
			data->files = data->files->next;
		}
		if ((new_lst = ft_lstnew(filename, ft_strlen(filename) + 1)) == NULL)
			return (-1);
		data->files->next = new_lst;
		data->files = tmp_lst;
	}
	else
		if ((data->files = ft_lstnew(filename, ft_strlen(filename) + 1)) == NULL)
			return (-1);
	return (0);
}
