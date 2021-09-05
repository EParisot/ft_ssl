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

static int 	PBKDF1(char *fct(char *, void *, size_t *), t_data *data, int32_t nIter, int dKlen, int hlen)
{
	int pass_len = ft_strlen((char*)data->pass);
	int l = (pass_len + 8 > hlen) ? pass_len + 8 : hlen;
	size_t dummy = l;
	unsigned char *DK = NULL;
	unsigned char *dk = NULL;
	
	if ((DK = malloc(l + 1)) == NULL)
		return -1;
	bzero(DK, l + 1);
	ft_memcpy(DK, data->pass, pass_len);
	ft_memcpy(DK + pass_len, data->salt, 8);
	for (int i = 0; i < nIter; ++i)
	{
		dummy = pass_len + 8;
		dk = (unsigned char *)fct((char*)DK, (void*)data, &dummy);
		if (read_hex((char*)dk, DK, hlen))
		{
			free(dk);
			return -1;
		}
		free(dk);
	}
	ft_memcpy(data->key, DK, dKlen);
	if (data->iv_provided == 0)
	{
		ft_memcpy(data->iv, DK + dKlen, dKlen);
	}
	free(DK);
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
	if (data->a_opt == 1)
	{
		char *b64_res = NULL;
		size_t len = ((t_string*)(data->strings->content))->len;
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
	else if (data->salted == 0 && data->key_provided == 0)
	{
		printf("ft_ssl : Error : No Key or Salt provided.\n");
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
		if (data->d_opt == 1)
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
		if (PBKDF1(sha256, data, 1, 8, 32))
			return -1;
	}
	if (data->iv_provided == 0 && is_empty(data->iv, 8))
	{
		random_value(data->iv, 8);
	}
	return 0;
}