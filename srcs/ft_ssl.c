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

int		hash_string(t_data *data)
{
	t_list	*tmp_lst;

	tmp_lst = data->strings;
	while (data->strings)
	{
		if (data->r_opt == 0 && data->q_opt == 0)
		{
			ft_putstr(data->hash->print_name);
			ft_putchar(' ');
			ft_putstr(((t_string *)(data->strings->content))->source);
			ft_putchar(' ');
		}
		if (data->hash && data->hash->func_ptr(((t_string *)(data->strings->content))->string))
			return (-1);
		if (data->r_opt == 1 && data->q_opt == 0)
		{
			ft_putchar(' ');
			ft_putendl(((t_string *)(data->strings->content))->source);
		}
		ft_putchar('\n');
		data->strings = data->strings->next;
	}
	data->strings = tmp_lst;
	return (0);
}