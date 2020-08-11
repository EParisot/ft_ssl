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

static int	get_s_opt(int ac, char **av, t_data data)
{
	if (ac > data->s_opt + 1 && (len = ft_strlen(av[data->s_opt + 1])))
	{
		{
			if ((data->string = (char *)malloc(len + 1)) == NULL)
				return (-1);
			ft_strcpy(data->string, av[data->s_opt + 1]);
			data->string[len] = 0;
			ft_putendl(data->string);
		}
		else
			print_help();
	}
	return (0);
}

static int	read_stdin(t_data *data)
{
	(void)data;
	return (0);
}

static int	parse_args(int ac, char **av, t_data *data)
{
	int 	i;
	int		len;

	i = 0;
	len = 0;
	if (ac > 1)
		while (++i < ac)
		{
			if (ft_strcmp(av[i], "-p") == 0)
				data->p_opt = 1;
			else if (ft_strcmp(av[i], "-q") == 0)
				data->q_opt = 1;
			else if (ft_strcmp(av[i], "-r") == 0)
				data->r_opt = 1;
			else if (ft_strcmp(av[i], "-s") == 0)
				data->s_opt = i;
			else if(ft_strcmp(av[i], "-h") == 0)
				print_help();
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

	if ((data = (t_data *)malloc(sizeof(t_data))) == NULL)
		return (-1);
	if (parse_args(ac, av, data))
		return (-1);
	free(data->string);
	free(data->hash);
	free(data);
	return (0);
}