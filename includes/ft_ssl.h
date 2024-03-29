/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H

# define FT_SSL_H

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

typedef enum		e_enc_type
{
	ENCRYPT,
	DECRYPT
}					t_enc_type;

typedef struct		s_string
{
	char			*string;
	char			*source;
	size_t			len;
	int				source_type;
}					t_string;

typedef struct		s_fct
{
	char			*name;
	char			*print_name;
	char			*(*func_ptr)(char *str, void *data, size_t *size);
}					t_fct;

typedef struct		s_data
{
	char 			*pass;
	unsigned char	key[9];
	unsigned char 	iv[9];
	unsigned char	salt[9];
	int				salted;
	int				key_provided;
	int				iv_provided;
	int 			a_opt;
	int				p_opt;
	int				q_opt;
	int				r_opt;
	int 			e_opt;
	int				d_opt;
	int				i_opt;
	int 			pksiv;
	FILE			*o_opt;
	t_fct			*hash;
	t_list			*strings;
}					t_data;

int 				read_hex(char *hex_str, unsigned char *out, int len);
void 				print_hex(unsigned char *hex, size_t size);
int 				bitstoint(char *binstr);
char 				*inttobits(int num, char *bits, size_t size);
char 				*str_to_bin(char *str, char *binstr);
char 				*bin_to_str(char *binstr, char *str);
void				xor_bin(char *a, char *b);
void				xor(char *a, char *b);
int					read_string(t_data *data);
int					get_string(t_data *data, char *str);
int					handle_files(t_data *data, char *filename);
int					hash_string(t_data *data);
void				print_help(int usage);
void				clean_data(t_data *data);
void				del(void *addr, size_t size);
char				*pad_len(char *str, size_t str_len, int *padded_size);
char				*add_len(char *padded_str, int *padded_size, uint64_t str_size, int swap);
uint32_t			rot_r(uint32_t x, uint32_t n);
uint32_t			rot_l(uint32_t x, uint32_t n);
char				*rot_str_l(char *str, int n);
int		 			random_value(unsigned char *r, size_t size);
void 				print_res(char *res, size_t size, FILE *fp);
int   				char_count(char *str, char c);
int 				is_empty(unsigned char *str, int len);

int 				securize(t_data* data);
char 				*append_salt(t_data *data, char *str);
int 				read_salt(t_data *data);

char				*md5(char *str, void *data, size_t *size);

char				*sha224(char *str, void *data, size_t *size);
char				*sha256(char *str, void *data, size_t *size);
int					sha256_loop(unsigned char *padded_str, int padded_size, char *str_res);
uint32_t			sha256_ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t			sha256_maj(uint32_t x, uint32_t y, uint32_t z);
uint32_t			sha256_bsig0(uint32_t x);
uint32_t			sha256_bsig1(uint32_t x);
uint32_t			sha256_ssig0(uint32_t x);
uint32_t			sha256_ssig1(uint32_t x);
uint32_t			sha256_k(int i);

char				*base64(char *str, void *data, size_t *size);

char				*des_cbc(char *str, void *data, size_t *size);
char				*des_ecb(char *str, void *data, size_t *size);

#endif
