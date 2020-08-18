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

# define MD5A 0x67452301
# define MD5B 0xefcdab89
# define MD5C 0x98badcfe
# define MD5D 0x10325476

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

int					md5(char *str);
int					md5f(int x, int y, int z);
int					md5g(int x, int y, int z);
int					md5h(int x, int y, int z);
int					md5i(int x, int y, int z);
uint32_t			md5ks(int i, char ks);

int					sha256(char *str);

#endif
