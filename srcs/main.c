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
	{"md5", "MD5", &md5},
	{"sha256", "SHA256", &sha256},
	{NULL, NULL, NULL}
};

static int	get_hash_or_file(t_data *data, char *hash_or_file)
{
	int i;

	i = 0;
	if (data->hash == NULL)
	{
		while (g_fcts[i].name)
		{
			if (ft_strcmp(g_fcts[i].name, hash_or_file) == 0)
			{
				if ((data->hash = (t_fct *)malloc(sizeof(t_fct))) == NULL)
					return (-1);
				ft_memmove(data->hash, &g_fcts[i], sizeof(t_fct));
				return (0);
			}
			++i;
		}
		ft_putstr("ft_ssl: Error: '");
		ft_putstr(hash_or_file);
		ft_putendl("' is an invalid command.");
		print_help(0, g_fcts);
		return (-1);
	}
	if (handle_files(data, hash_or_file) == -1)
		return (-1);
	return (0);
}

static int	get_strings(t_data *data, char *str)
{
	(void)data;
	(void)str;
	// TODO
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
			{
				if (read_stdin(data))
					return (-1);
				data->p_opt = 1;
			}
			else if (ft_strcmp(av[i], "-q") == 0)
				data->q_opt = 1;
			else if (ft_strcmp(av[i], "-r") == 0)
				data->r_opt = 1;
			else if (ft_strcmp(av[i], "-s") == 0 && ac > i + 1)
			{
				if (get_strings(data, av[++i]))
					return (-1);
			}
			else if (ft_strcmp(av[i], "-h") == 0)
				print_help(1, g_fcts);
			else if (get_hash_or_file(data, av[i]))
				return (-1);
		}
	return (0);
}

static int	init_env(t_data *data, int ac, char **av)
{
	if (parse_args(ac, av, data))
		return (-1);
	if (data->hash && data->strings == NULL)
	{
		if (read_stdin(data))
			return (-1);
	}
	else if (data->hash == NULL)
	{
		print_help(1, g_fcts);
		return (-1);
	}
	return (0);
}

int			main(int ac, char **av)
{
	t_data	*data;
	int		ret;

	if ((data = (t_data *)malloc(sizeof(t_data))) == NULL)
		return (-1);
	ft_memset(data, 0, sizeof(t_data));
	ret = init_env(data, ac, av);
	if (ret == 0 && hash_string(data))
		ret = -1;
	clean_data(data);
	return (ret);
}
