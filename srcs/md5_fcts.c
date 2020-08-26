/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_fcts.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

uint32_t			md5f(uint32_t x, uint32_t y, uint32_t z)
{
	return ((x & y) | ((~x) & z));
}

uint32_t			md5g(uint32_t x, uint32_t y, uint32_t z)
{
	return ((x & z) | (y & (~z)));
}

uint32_t			md5h(uint32_t x, uint32_t y, uint32_t z)
{
	return (x ^ y ^ z);
}

uint32_t			md5i(uint32_t x, uint32_t y, uint32_t z)
{
	return (y ^ (x | (~z)));
}
