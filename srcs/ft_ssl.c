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
	if (data->stdin)
		data->hash->func_ptr(data->stdin);
	if (data->string)
		data->hash->func_ptr(data->string);
	if (data->files)
	{
		if (read_files(data))
			ft_putendl("Error reading file");
	}
	return (0);
}