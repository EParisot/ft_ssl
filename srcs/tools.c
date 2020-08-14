/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

void				del(void *addr, size_t size)
{
	(void)size;
	free(addr);
}

void				clean_data(t_data *data)
{
	if (data && data->files)
		ft_lstdel(&data->files, del);
	if (data && data->string)
		free(data->string);
	if (data && data->hash)
		free(data->hash);
	free(data);
}
