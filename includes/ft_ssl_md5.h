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

# define BUF_SIZE 64

typedef enum		e_source_type
{
	_STRING,
	_STDIN,
	_FILE
}					t_source_type;

typedef struct		s_fct
{
	char			*name;
	char			*print_name;
	int				(*func_ptr)(char *str);
}					t_fct;

typedef struct		s_string
{
	char			*string;
	char			*source;
	int				source_type;
}					t_string;

typedef struct		s_data
{
	int				p_opt;
	int				q_opt;
	int				r_opt;
	t_fct			*hash;
	t_list			*strings;
}					t_data;

int					read_stdin(t_data *data);
int					handle_files(t_data *data, char *filename);
int					hash_string(t_data *data);
void				print_help(int usage, t_fct *g_fcts);
void				clean_data(t_data *data);
void				del(void *addr, size_t size);

char				*pad_len(char *str, int *padded_size);
char				*add_len(char *padded_str, int *padded_size, \
								uint64_t str_size, int swap);

uint32_t			rot_r(uint32_t x, uint32_t n);
uint32_t			rot_l(uint32_t x, uint32_t n);

int					md5(char *str);
uint32_t			md5f(uint32_t x, uint32_t y, uint32_t z);
uint32_t			md5g(uint32_t x, uint32_t y, uint32_t z);
uint32_t			md5h(uint32_t x, uint32_t y, uint32_t z);
uint32_t			md5i(uint32_t x, uint32_t y, uint32_t z);
uint32_t			md5k(int i);
uint32_t			md5t(int i);
uint32_t			md5s(int i);

int					sha256(char *str);
uint32_t			sha256ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t			sha256maj(uint32_t x, uint32_t y, uint32_t z);
uint32_t			sha256_bsig0(uint32_t x);
uint32_t			sha256_bsig1(uint32_t x);
uint32_t			sha256_ssig0(uint32_t x);
uint32_t			sha256_ssig1(uint32_t x);
uint32_t			shak(int i);

#endif
