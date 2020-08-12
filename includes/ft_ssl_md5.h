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

typedef struct		s_fct
{
	char			*name;
	char			*print_name;
}					t_fct;

typedef struct		s_data
{
	int				p_opt;
	int				q_opt;
	int				r_opt;
	int				s_opt;
	t_fct			*hash;
	char			*string;
}					t_data;

int					read_stdin(t_data *data, t_fct *g_fcts);

#endif
