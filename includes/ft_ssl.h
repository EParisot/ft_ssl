/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_md5.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_MD5_H

# define FT_SSL_MD5_H

# include "../libft/libft.h"
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdint.h>
# include <stdio.h>

# define BUF_SIZE 64
# define BASE64_ALPHA "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

typedef enum		e_source_type
{
	_STRING,
	_STDIN,
	_FILE
}					t_source_type;

typedef struct		s_string
{
	char			*string;
	char			*source;
	int				source_type;
}					t_string;

typedef struct		s_fct
{
	char			*name;
	char			*print_name;
	char			*(*func_ptr)(char *str, void *data, int print);
}					t_fct;

typedef struct		s_data
{
	char 			*pass;
	unsigned char	key[9];
	unsigned char   des_key[9];
	unsigned char 	iv[9];
	unsigned char	salt[9];
	int 			a_opt;
	int				p_opt;
	int				q_opt;
	int				r_opt;
	int 			e_opt;
	int				d_opt;
	int				i_opt;
	FILE			*o_opt;
	t_fct			*hash;
	t_list			*strings;
}					t_data;

int 				read_hex(char *hex_str, unsigned char *out);
void 				print_hex(unsigned char *hex, size_t size);
int					read_string(t_data *data);
int					read_stdin(char *buf, int size);
int					handle_files(t_data *data, char *filename);
int					hash_string(t_data *data);
void				print_help(int usage, t_fct *g_fcts);
void				clean_data(t_data *data);
void				del(void *addr, size_t size);

char				*pad_len(char *str, int *padded_size);
char				*add_len(char *padded_str, int *padded_size, uint64_t str_size, int swap);

uint32_t			rot_r(uint32_t x, uint32_t n);
uint32_t			rot_l(uint32_t x, uint32_t n);

int 				securize(t_data* data);

char				*md5(char *str, void *data, int print);

char				*sha224(char *str, void *data, int print);
char				*sha256(char *str, void *data, int print);
int					sha256_loop(unsigned char *padded_str, int padded_size, char *str_res);
uint32_t			sha256_ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t			sha256_maj(uint32_t x, uint32_t y, uint32_t z);
uint32_t			sha256_bsig0(uint32_t x);
uint32_t			sha256_bsig1(uint32_t x);
uint32_t			sha256_ssig0(uint32_t x);
uint32_t			sha256_ssig1(uint32_t x);
uint32_t			sha256_k(int i);

char				*base64(char *str, void *data, int print);
void 				b64_encode_str(char *str, char **converted);
void 				b64_decode_str(char *str, char **converted);

char				*des_cbc(char *str, void *data, int print);
char				*des_ecb(char *str, void *data, int print);

#endif
