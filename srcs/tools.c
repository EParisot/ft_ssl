/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

void			print_help(int usage, t_fct *g_fcts)
{
	int i;

	i = 0;
	if (usage)
		ft_putendl("usage: ./ft_ssl [hash] [opt] [string]");
	else
	{
		ft_putendl("\nMessage Digest commands:");
		while (g_fcts[i].name)
			ft_putendl(g_fcts[i++].name);
	}
}

void			del(void *addr, size_t size)
{
	(void)size;
	if (((t_string *)addr)->string)
		free(((t_string *)addr)->string);
	if (((t_string *)addr)->source)
		free(((t_string *)addr)->source);
	free(addr);
}

void			clean_data(t_data *data)
{
	if (data && data->pass)
		free(data->pass);
	if (data && data->strings)
		ft_lstdel(&data->strings, del);
	if (data && data->hash)
		free(data->hash);
	free(data);
}

uint32_t		rot_r(uint32_t x, uint32_t n)
{
	return ((x >> n) | (x << (32 - n)));
}

uint32_t		rot_l(uint32_t x, uint32_t n)
{
	return ((x << n) | (x >> (32 - n)));
}

void 			print_hex(unsigned char *hex, size_t size)
{
	printf("0x");
	for (size_t i = 0; i < size; i++)
	{
		printf("%02x", hex[i]);
	}
	printf("\n");
}

int 			random_value(unsigned char *r, size_t size)
{
	int fd = 0;

	if ((fd = open("/dev/urandom", O_RDONLY)) < 0)
		return -1;
	if (read(fd, r, size) < 0)
		return -1;
	for (size_t i = 0; i < size; i++)
	{
		if (r[i] >= 255)
			r[i] = r[i] % 255;
	}
	close(fd);
	return 0;
}

static void 	PBKDF2(int fct(char *, void *), t_data *data, int c, int d)
{//https://fr.wikipedia.org/wiki/PBKDF2
	(void)fct;
	(void)data;
	(void)c;
	(void)d;
	if (data->pass)
		free(data->pass);
	data->pass = NULL;
}

int 			securize(t_data *data)
{
	char *tmp = NULL;
	char *tmp0 = NULL;
	char *tmp1 = NULL;

	if (ft_strcmp((char *)data->key, "\0") == 0)
	{
		while (data->pass == NULL)
		{
			if ((tmp = getpass("\nPlease type a password: ")) == NULL)
				return -1;
			if ((tmp0 = ft_strdup(tmp)) == NULL)
				return -1;
			if ((tmp1 = ft_strdup(getpass("Please type password again: "))) == NULL)
				return -1;
			if (ft_strcmp(tmp0, tmp1) == 0)
			{
				if ((data->pass = ft_strdup(tmp1)) == NULL)
					return -1;
			}
			else
			{
				printf("Password confirmation failed, try again...\n");
			}
			free(tmp0);
			free(tmp1);
		}
		free(tmp);
		PBKDF2(sha256, data, 256, 64);
	}
	if (ft_strcmp((char *)data->salt, "\0") == 0)
	{
		random_value(data->salt, 8);
	}
	if (ft_strcmp((char *)data->iv, "\0") == 0)
	{
		random_value(data->iv, 8);
	}
	printf("Key  : ");
	print_hex(data->key, 8);
	printf("Salt : ");
	print_hex(data->salt, 8);
	printf("IV   : ");
	print_hex(data->iv, 8);
	return 0;
}