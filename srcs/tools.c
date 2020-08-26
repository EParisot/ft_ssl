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

void			print_help(int usage, t_fct *g_fcts)
{
	int i;

	i = 0;
	if (usage)
		ft_putendl("usage: ./ft_ssl [hash] [opt] [string]");
	else
	{
		ft_putendl("\nMessage Digest commands:");
		while (g_fcts[i].name)
			ft_putendl(g_fcts[i++].name);
	}
}

void			del(void *addr, size_t size)
{
	(void)size;
	if (((t_string *)addr)->string)
		free(((t_string *)addr)->string);
	if (((t_string *)addr)->source)
		free(((t_string *)addr)->source);
	free(addr);
}

void			clean_data(t_data *data)
{
	if (data && data->strings)
		ft_lstdel(&data->strings, del);
	if (data && data->hash)
		free(data->hash);
	free(data);
}

uint32_t		rot_r(uint32_t x, uint32_t n)
{
	return ((x >> n) | (x << (32 - n)));
}

uint32_t		rot_l(uint32_t x, uint32_t n)
{
	return ((x << n) | (x >> (32 - n)));
}
