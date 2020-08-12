/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

static t_fct	g_fcts[] = {
	{"test", "TEST"},
	{NULL, NULL}
};

static void	print_help(void)
{
	int i;

	i = 0;
	ft_putendl("usage: ./ft_ssl [hash] [opt] [string]");
	ft_putendl("\nMessage Digest commands");
	while (g_fcts[i].name)
		ft_putendl(g_fcts[i++].name);
}

static int	set_hash_or_file(t_data *data, char *hash_or_file)
{
	int i;

	i = 0;
	while (g_fcts[i].name)
	{
		if (ft_strcmp(g_fcts[i].name, hash_or_file) == 0)
		{
			if ((data->hash = (t_fct *)malloc(sizeof(t_fct))) == NULL)
				return (-1);
			ft_memmove(data->hash, g_fcts[i].name, sizeof(t_fct));
			return (0);
		}
		++i;
	}
	if (handle_files(data, hash_or_file))
		return (-1);
	return (0);
}

static int	get_s_opt(int ac, char **av, t_data *data)
{
	int len;

	len = 0;
	if (ac > data->s_opt + 1 && (len = ft_strlen(av[data->s_opt + 1])))
	{
		if ((data->string = (char *)malloc(len + 1)) == NULL)
			return (-1);
		ft_strcpy(data->string, av[data->s_opt + 1]);
		data->string[len] = 0;
	}
	else
		print_help();
	return (0);
}

static int	parse_args(int ac, char **av, t_data *data)
{
	int		i;

	i = 0;
	if (ac > 1)
		while (++i < ac)
		{
			if (ft_strcmp(av[i], "-p") == 0)
				data->p_opt = 1;
			else if (ft_strcmp(av[i], "-q") == 0)
				data->q_opt = 1;
			else if (ft_strcmp(av[i], "-r") == 0)
				data->r_opt = 1;
			else if (ft_strcmp(av[i], "-s") == 0 && ac > i + 1)
				data->s_opt = i++;
			else if (ft_strcmp(av[i], "-h") == 0)
				print_help();
			else if (set_hash_or_file(data, av[i]))
				return (-1);
		}
	if (data->s_opt)
		if (get_s_opt(ac, av, data))
			return (-1);
	return (0);
}

int			main(int ac, char **av)
{
	t_data	*data;
	int		ret;

	ret = 0;
	if ((data = (t_data *)malloc(sizeof(t_data))) == NULL)
		return (-1);
	ft_memset(data, 0, sizeof(t_data));
	if (parse_args(ac, av, data))
		return (-1);
	if (data->s_opt == 0 && data->string == NULL && data->files)
	{
		if (read_files(data))
			ft_putendl("Error reading file");
	}
	else if (data->s_opt == 0 && data->string == NULL)
	{
		if (read_stdin(data))
			ret = -1;
	}
	if (hash_string(data))
		ret = -1;
	if (data->files)
		ft_lstdel(&data->files, del);
	if (data->string)
		free(data->string);
	free(data->hash);
	free(data);
	return (ret);
}
