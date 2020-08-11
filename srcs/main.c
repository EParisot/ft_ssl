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

static void	print_help(void)
{
	ft_putendl("usage: ./ft_ssl [hash] [opt] [string]");
	ft_putendl("\nMessage Digest commands");
}

static int	set_hash(t_data *data, t_fct *fcts, char *hash)
{
	int i;

	i = 0;
	while (fcts[i].name)
	{
		if (ft_strcmp(fcts[i].name, hash) == 0)
		{
			if ((data->hash = (t_fct *)malloc(sizeof(t_fct))) == NULL)
				return (-1);
			ft_memmove(data->hash, fcts[i].name, sizeof(t_fct));
		}
		++i;
	}
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
		ft_putendl(data->string);
	}
	else
		print_help();
	return (0);
}

static int	parse_args(int ac, char **av, t_data *data, t_fct *fcts)
{
	int 	i;

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
			else if(ft_strcmp(av[i], "-h") == 0)
				print_help();
			else if (set_hash(data, fcts, av[i]))
				return (-1);
		}
	else if (read_stdin(data))
		return (-1);
	if (data->s_opt)
		if (get_s_opt(ac, av, data))
			return (-1);
	return (0);
}

int 		main(int ac, char **av)
{
	t_data	*data;

	t_fct	fcts[] = {
		{"test", "TEST"},
		{NULL, NULL}
	};
	if ((data = (t_data *)malloc(sizeof(t_data))) == NULL)
		return (-1);
	if (parse_args(ac, av, data, fcts))
		return (-1);
	free(data->string);
	free(data->hash);
	free(data);
	return (0);
}