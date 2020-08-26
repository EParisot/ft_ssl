/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha512_fcts_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

uint64_t	sha512_bsig0(uint64_t x)
{
	return (rot_r(x, 28) ^ rot_r(x, 34) ^ rot_r(x, 39));
}

uint64_t	sha512_bsig1(uint64_t x)
{
	return (rot_r(x, 14) ^ rot_r(x, 18) ^ rot_r(x, 41));
}

uint64_t	sha512_ssig0(uint64_t x)
{
	return (rot_r(x, 1) ^ rot_r(x, 8) ^ (x >> 7));
}

uint64_t	sha512_ssig1(uint64_t x)
{
	return (rot_r(x, 19) ^ rot_r(x, 61) ^ (x >> 6));
}
