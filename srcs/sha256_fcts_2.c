/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256_fcts_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl_md5.h"

uint32_t	sha256_bsig0(uint32_t x)
{
	return (rot_r(x, 2) ^ rot_r(x, 13) ^ rot_r(x, 22));
}

uint32_t	sha256_bsig1(uint32_t x)
{
	return (rot_r(x, 6) ^ rot_r(x, 11) ^ rot_r(x, 25));
}

uint32_t	sha256_ssig0(uint32_t x)
{
	return (rot_r(x, 7) ^ rot_r(x, 18) ^ (x >> 3));
}

uint32_t	sha256_ssig1(uint32_t x)
{
	return (rot_r(x, 17) ^ rot_r(x, 19) ^ (x >> 10));
}
