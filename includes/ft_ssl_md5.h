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

# define BUF_SIZE 64

typedef struct		s_fct
{
	char			*name;
	char			*print_name;
	int				(*func_ptr)(char *str);
}					t_fct;

typedef struct		s_data
{
	int				p_opt;
	int				q_opt;
	int				r_opt;
	int				s_opt;
	t_fct			*hash;
	char			*string;
	char			*stdin;
	t_list			*files;
}					t_data;

int					read_stdin(t_data *data);
int 				handle_files(t_data *data, char *filename);
int					read_files(t_data *data);
int					hash_string(t_data *data);
int					md5(char *str);
int					sha256(char *str);
void				print_help(int usage, t_fct *g_fcts);
void				clean_data(t_data *data);
void				del(void *addr, size_t size);

#endif
