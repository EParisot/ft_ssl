/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

static void	prefix(t_data *data, int source_type)
{
	if (source_type == _STDIN && data->p_opt == 1 && data->q_opt == 0)
		ft_putstr(((t_string *)(data->strings->content))->string);
	else if (source_type != _STDIN && data->r_opt == 0 && data->q_opt == 0)
	{
		ft_putstr(data->hash->print_name);
		if (((t_string *)(data->strings->content))->source ||
			((t_string *)(data->strings->content))->string)
		{
			ft_putstr(" (");
			if (source_type == _FILE)
				ft_putstr(((t_string *)(data->strings->content))->source);
			else
			{
				ft_putchar('"');
				ft_putstr(((t_string *)(data->strings->content))->string);
				ft_putchar('"');
			}
			ft_putstr(") =");
		}
		ft_putchar(' ');
	}
}

static void	suffix(t_data *data, int source_type)
{
	if (source_type != _STDIN && data->r_opt == 1 && data->q_opt == 0)
	{
		ft_putchar(' ');
		if (source_type == _FILE)
			ft_putstr(((t_string *)(data->strings->content))->source);
		else
		{
			ft_putchar('"');
			ft_putstr(((t_string *)(data->strings->content))->string);
			ft_putchar('"');
		}
	}
}

int			hash_string(t_data *data)
{
	t_list	*tmp_lst;

	tmp_lst = data->strings;
	while (data->strings)
	{
		prefix(data, ((t_string *)(data->strings->content))->source_type);
		if (data->hash && \
		data->hash->func_ptr(((t_string *)(data->strings->content))->string))
			return (-1);
		suffix(data, ((t_string *)(data->strings->content))->source_type);
		ft_putchar('\n');
		data->strings = data->strings->next;
	}
	data->strings = tmp_lst;
	return (0);
}
