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

static char *HMAC(char *input, char *fct(char *, void *, size_t *), t_data *data, int dKlen)
{// TODO
	(void)dKlen;
	return fct((char*)input, (void*)data, NULL);
}

static int 	PBKDF2(char *fct(char *, void *, size_t *), t_data *data, int32_t nIter, int dKlen)
{//https://fr.wikipedia.org/wiki/PBKDF2
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
				int input_len = ft_strlen((char*)data->pass) + 8 + sizeof(nIter) + 1;
				if ((input = malloc(input_len)) == NULL)
					return -1;
				bzero((char*)input, input_len);
				ft_strcpy((char*)input, (char*)data->pass);
				ft_memmove((char*)input + ft_strlen((char*)data->pass), (char*)data->salt, 8);
				ft_memmove((char*)input + ft_strlen((char*)data->pass) + 8, (char*)&nIter, sizeof(nIter));
				//printf("%s\n", (char*)input);
				//print_hex(input, input_len);
				char *hash = HMAC((char*)input, fct, data, dKlen);
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
				char *hash = HMAC((char*)input, fct, data, dKlen);
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

char 			*append_salt(t_data *data, char *str)
{
	char *new_str = NULL;

	if ((new_str = malloc(16 + ((t_string *)(data->strings->content))->len + 1)) == NULL)
	{
		return NULL;
	}
	bzero(new_str, 16 + ((t_string *)(data->strings->content))->len + 1);
	ft_memcpy(new_str, "Salted__", 8);
	ft_memcpy(new_str + 8, (char*)data->salt, 8);
	ft_memcpy(new_str + 16, str, ((t_string *)(data->strings->content))->len);
	free(str);
	((t_string *)(data->strings->content))->len += 16;
	return new_str;
}

int  			read_salt(t_data *data)
{
	char *b64_res = NULL;
	size_t len = ((t_string*)(data->strings->content))->len;

	if (data->a_opt == 1)
	{
		char *str = ft_strdup(((t_string*)(data->strings->content))->string);
		if ((b64_res = base64(str, data, &len)) == NULL)
			return -1;
		if (ft_strncmp(b64_res, "Salted__", 8) != 0)
		{
			printf("ft_ssl : Error : Salt not found\n");
			free(b64_res);
			free(str);
			return -1;
		}
		ft_memcpy((char*)data->salt, b64_res + 8, 8);
		free(b64_res);
		free(str);
	}
	else if (ft_strncmp(((t_string *)(data->strings->content))->string, "Salted__", 8) == 0)
	{
		ft_memcpy((char*)data->salt, ((t_string *)(data->strings->content))->string + 8, 8);
		ft_memmove(((t_string *)(data->strings->content))->string, ((t_string *)(data->strings->content))->string + 16, ((t_string *)(data->strings->content))->len - 16);
		((t_string *)(data->strings->content))->len -= 16;
	}
	else
	{
		printf("ft_ssl : Error : No salt provided.\n");
		return -1;
	}
	return 0;
}

int 			securize(t_data *data)
{
	char *tmp = NULL;
	char *tmp0 = NULL;
	char *tmp1 = NULL;

	if (is_empty(data->key, 8) && data->salted == 0)
	{
		if (data->e_opt == 1 && data->salted == 0)
		{
			random_value(data->salt, 8);
		}
		else
		{
			if (read_salt(data))
				return -1;
		}
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
		if (PBKDF2(sha256, data, 10000, 64))
			return -1;
	}
	if (is_empty(data->iv, 8))
	{
		random_value(data->iv, 8);
	}
	return 0;
}