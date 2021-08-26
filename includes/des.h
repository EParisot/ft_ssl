/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DES_H

#define DES_H

# include "ft_ssl.h"

typedef struct	s_des
{
	char 		bin_buf[9 * 8];
	char 		ip_res[65];
	char 		l_block[33];
	char 		r_block[33];
	char 		Ln[33];
	char 		Rn[33];
	char 		Ln_1[33];
	char 		Rn_1[33];
	char 		Fn[65];
	char 		bin_res[65];
}				t_des;

extern 	int kperm[56];
extern 	int key_comp[48];
extern 	int exp_d[48];
extern 	int s[8][4][16];
extern 	int per[32];
extern 	int initial_perm[64];
extern 	int final_perm[64];

void 	preprocess_key(t_data *data, char *key);
int 	subkeys_routine(char *key, char **keys);
char 	*preprocess_message(char *str, size_t *str_len, int mode);
void 	postprocess_message(char *str, size_t *str_len);
void  	f_function(char *r, char *key, char *res);
void 	des_clean(char **keys, char *message);

#endif