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
	if (data->r_opt)
	{
		if (data->p_opt || data->q_opt == 0)
		{
			ft_putstr(data->string);
			ft_putchar(' ');
			ft_putendl(data->hash->print_name);
		}
		
	}
	else
	{
		if (data->p_opt || data->q_opt == 0)
		{
			ft_putstr(data->hash->print_name);
			ft_putchar(' ');
			ft_putendl(data->string);
		}
	}
	data->hash->func_ptr((void*)data);
	return (0);
}