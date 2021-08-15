/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_ecb.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 17:12:42 by eparisot          #+#    #+#             */
/*   Updated: 2021/08/07 17:12:42 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"
//https://www.geeksforgeeks.org/data-encryption-standard-des-set-1/
int 				des_ecb(char *str, void *data)
{
	t_data *d = (t_data *)data;

	printf("str=%s pass=%s key=%s salt=%s iv=%s\n", str, d->pass, d->key, d->salt, d->iv);
	return 0;
}