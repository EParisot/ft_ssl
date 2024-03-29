/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

static t_fct	g_fcts[] = {
	{"md5", "MD5", &md5},
	{"sha224", "SHA224", &sha224},
	{"sha256", "SHA256", &sha256},
	{"base64", "BASE64", &base64},
	{"des", "DES", &des_cbc},
	{"des-cbc", "DES_CBC", &des_cbc},
	{"des-ecb", "DES_ECB", &des_ecb},
	{NULL, NULL, NULL}
};

static int	get_hash_or_file(t_data *data, char *hash_or_file, int i)
{
	if (data->hash == NULL)
	{
		while (g_fcts[i].name)
		{
			if (ft_strcmp(g_fcts[i].name, hash_or_file) == 0)
			{
				if ((data->hash = (t_fct *)malloc(sizeof(t_fct))) == NULL)
					return (-1);
				ft_memmove(data->hash, &g_fcts[i], sizeof(t_fct));
				return (0);
			}
			++i;
		}
		printf("ft_ssl: Error: '%s' is an invalid command.", hash_or_file);
		print_help(0);
		return (-1);
	}
	if (handle_files(data, hash_or_file))
		return (-1);
	return (0);
}

static int	parse_args(int ac, char **av, t_data *data, int i)
{
	if (ac > 1)
		while (++i < ac)
		{
			if (ft_strcmp(av[i], "-q") == 0)
				data->q_opt = 1;
			else if (ft_strcmp(av[i], "-r") == 0)
				data->r_opt = 1;
			else if (ft_strcmp(av[i], "-e") == 0)
				data->e_opt = 1;
			else if (ft_strcmp(av[i], "-d") == 0)
			{
				data->e_opt = 0;
				data->d_opt = 1;
			}
			else if (ft_strcmp(av[i], "-p") == 0)
			{
				if (data->hash && ft_strncmp(data->hash->name, "des", 3) == 0)
				{
					if (ac > i + 1)
					{
						if ((data->pass = malloc(ft_strlen(av[i+1]) + 1)) == NULL)
							return -1;
						ft_strcpy(data->pass, av[++i]);
					}
				}
				else
				{
					if (read_string(data))
						return (-1);
					data->p_opt = 1;
				}
			}
			else if (ft_strcmp(av[i], "-s") == 0)
			{
				if (ac > i + 1)
				{
					if (data->hash && ft_strncmp(data->hash->name, "des", 3) == 0)
					{
						if (ac > i + 1)
						{
							read_hex(av[++i], data->salt, 8);
							data->salted = 1;
						}
						else
						{
							print_help(1);
							return (-1);
						}
					}
					else
					{
						if (ac > i + 1)
						{
							if (get_string(data, av[++i]))
								return (-1);
						}
						else
						{
							print_help(1);
							return (-1);
						}
					}
				}
				else
				{
					print_help(1);
						return (-1);
				}
			}
			else if (ft_strcmp(av[i], "-a") == 0)
			{
				if (data->hash && ft_strncmp(data->hash->name, "des", 3) == 0)
				{
					data->a_opt = 1;
				}
			}
			else if (ft_strcmp(av[i], "-k") == 0)
			{
				if (data->hash && ft_strncmp(data->hash->name, "des", 3) == 0)
				{
					if (ac > i + 1)
					{
						read_hex(av[++i], data->key, 8);
						data->key_provided = 1;
					}
				}
			}
			else if (ft_strcmp(av[i], "-v") == 0)
			{
				if (data->hash && ft_strncmp(data->hash->name, "des", 3) == 0)
				{
					if (ac > i + 1)
					{
						read_hex(av[++i], data->iv, 8);
						data->iv_provided = 1;
					}
				}
			}
			else if (ft_strcmp(av[i], "-P") == 0)
			{
				data->pksiv = 1;
			}
			else if (ft_strcmp(av[i], "-i") == 0)
			{
				if (ac > i + 1)
				{
					if (handle_files(data, av[++i]))
						return (-1);
				}
				else
				{
					print_help(1);
					return (-1);
				}
			}
			else if (ft_strcmp(av[i], "-o") == 0)
			{
				if (ac > i + 1)
				{
					FILE *fd;
					if ((fd = fopen(av[++i], "w")) == NULL)
					{
						printf("ft_ssl: Error: cannot open '%s' for writing.", av[i+1]);
						return -1;
					}
					data->o_opt = fd;
				}
				else
				{
					print_help(1);
					return (-1);
				}
			}
			else if (ft_strcmp(av[i], "-h") == 0)
			{
				print_help(1);
				return (-1);
			}
			else if (get_hash_or_file(data, av[i], 0))
				return (-1);
		}
	return (0);
}

static int	init_env(t_data *data, int ac, char **av)
{
	data->e_opt = 1;
	data->o_opt = stdout;
	if (parse_args(ac, av, data, 0))
		return (-1);
	if (data->hash && data->strings == NULL)
	{
		if (read_string(data))
			return (-1);
	}
	else if (data->hash == NULL)
	{
		print_help(1);
		return (-1);
	}
	return (0);
}

int			main(int ac, char **av)
{
	t_data	*data;
	int		ret;

	if ((data = (t_data *)malloc(sizeof(t_data))) == NULL)
		return (-1);
	ft_memset(data, 0, sizeof(t_data));
	ret = init_env(data, ac, av);
	if (ret == 0 && hash_string(data))
		ret = -1;
	clean_data(data);
	return (ret);
}
