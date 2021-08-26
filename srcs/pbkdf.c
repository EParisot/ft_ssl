/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pbkdf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

static int 	PBKDF2(char *fct(char *, void *, int), t_data *data, int nIter, int dKlen)
{//https://fr.wikipedia.org/wiki/PBKDF2
	int print = 0;
	int hlen = 256;
	int l = (dKlen + hlen - 1) / hlen;
	unsigned char *input = NULL;
	unsigned char *dk = NULL;
	unsigned char *DK = NULL;
	unsigned char *output = NULL;
	if ((dk = malloc(hlen)) == NULL)
		return -1;
	if ((DK = malloc(l * hlen)) == NULL)
		return -1;
	if ((output = malloc(hlen + 1)) == NULL )
		return -1;
	bzero(DK, l * hlen);
	for (int i = 0; i < l; ++i)
	{
		bzero(dk, hlen);
		for (int j = 0; j < nIter; ++j)
		{
			if (j == 0)
			{
				bzero((char*)output, hlen + 1);
				int input_len = ft_strlen((char*)data->pass) + 8 + sizeof(i) + 1;
				if ((input = malloc(input_len)) == NULL )
					return -1;
				bzero((char*)input, input_len);
				ft_strcpy((char*)input, (char*)data->pass);
				ft_memmove((char*)input + ft_strlen((char*)data->pass), (char*)data->salt, 8);
				ft_memmove((char*)input + ft_strlen((char*)data->pass) + 8, (char*)&i, sizeof(i));
				//printf("%s\n", (char*)input);
				//print_hex(input, input_len);
				char *hash = fct((char*)input, (void*)data, print);
				if (read_hex(hash, output))
					return -1;
				//print_hex(input, input_len);
				//printf("%ld %s %ld %s\n", ft_strlen((char*)input), (char*)input, ft_strlen((char*)output), (char*)output);
				for (int x = 0; x < hlen; ++x)
				{
					dk[x] ^= output[x];
				}
				free(input);
				free(hash);
			}
			else
			{
				int input_len = ft_strlen((char*)data->pass) + hlen + 1;
				if ((input = malloc(input_len)) == NULL )
					return -1;
				bzero((char*)input, input_len);
				ft_strcpy((char*)input, (char*)data->pass);
				ft_memmove((char*)input + ft_strlen((char*)data->pass), (char*)output, hlen);
				bzero((char*)output, hlen);
				//printf("%s\n", (char*)input);
				char *hash = fct((char*)input, (void*)data, print);
				if (read_hex(hash, output))
					return -1;
				//printf("%ld %s %ld %s\n", ft_strlen((char*)input), (char*)input, ft_strlen((char*)output), (char*)output);
				for (int x = 0; x < hlen; ++x)
				{
					dk[x] ^= output[x];
				}
				free(input);
				free(hash);
			}
		}
		ft_strcat((char*)DK, (char*)dk);
	}
	ft_strncpy((char *)data->key, (char *)DK, 8);
	free(output);
	free(dk);
	free(DK);
	if (data->pass)
		free(data->pass);
	data->pass = NULL;
	return 0;
}

int 			is_empty(unsigned char *str, int len)
{
	for (int i = 0; i < len; ++i)
	{
		if (str[i] != 0)
			return 0;
	}
	return 1;
}

int 			securize(t_data *data)
{
	char *tmp = NULL;
	char *tmp0 = NULL;
	char *tmp1 = NULL;

	if (is_empty(data->salt, 8))
	{
		random_value(data->salt, 8);
	}
	if (is_empty(data->key, 8))
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
		if (PBKDF2(sha256, data, 256, 64))
			return -1;
	}
	if (is_empty(data->iv, 8))
	{
		random_value(data->iv, 8);
	}
	/*printf("Key  : ");
	print_hex(data->key, 8);
	printf("Salt : ");
	print_hex(data->salt, 8);
	printf("IV   : ");
	print_hex(data->iv, 8);*/
	return 0;
}