/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 23:09:42 by eparisot          #+#    #+#             */
/*   Updated: 2020/08/11 23:11:02 by eparisot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

static void	prefix(t_data *data, int source_type)
{
	if (source_type == _STDIN && data->p_opt == 1 && data->q_opt == 0)
	{
		fprintf(stderr, "%s", ((t_string *)(data->strings->content))->string);
		if (((t_string *)(data->strings->content))->string[ft_strlen(((t_string *)(data->strings->content))->string) - 1] != '\n')
			fprintf(stderr, "\n");
	}
	else if (data->r_opt == 0 && data->q_opt == 0)
	{
		if (source_type != _STDIN)
			fprintf(stderr, "%s ", data->hash->print_name);
		if (((t_string *)(data->strings->content))->source ||
			((t_string *)(data->strings->content))->string)
		{
			if (source_type == _STDIN)
			{
				fprintf(stderr, "(stdin)= ");
			}
			else if (source_type == _FILE)
			{
				fprintf(stderr, "(%s)= ", ((t_string *)(data->strings->content))->source);
			}
			else
			{
				if (data->hex)
				{
					fprintf(stderr, "(\"");
					print_hex((unsigned char *)((t_string *)(data->strings->content))->string, ft_strlen(((t_string *)(data->strings->content))->string));
					fprintf(stderr, "\")= ");
				}
				else
					fprintf(stderr, "(\"%s\")= ", ((t_string *)(data->strings->content))->string);
			}
		}
	}
}

static void	suffix(t_data *data, int source_type)
{
	if (data->r_opt == 1 && data->q_opt == 0)
	{
		if (source_type == _STDIN)
			fprintf(stderr, " (stdin)");
		else if (source_type == _FILE)
			fprintf(stderr, " %s", ((t_string *)(data->strings->content))->source);
		else
		{
			if (data->hex)
				{
					fprintf(stderr, " (\"");
					print_hex((unsigned char *)((t_string *)(data->strings->content))->string, ft_strlen(((t_string *)(data->strings->content))->string));
					fprintf(stderr, "\")");
				}
				else
					fprintf(stderr, " (\"%s\")", ((t_string *)(data->strings->content))->string);
		}
	}
}

int			hash_string(t_data *data)
{
	t_list	*tmp_lst;
	char 	*res = NULL;

	tmp_lst = data->strings;
	while (data->strings)
	{
		if (data->o_opt == stdout)
		{
			prefix(data, ((t_string *)(data->strings->content))->source_type);
			if (data->hash && (res = data->hash->func_ptr(((t_string *)(data->strings->content))->string, data, &((t_string *)(data->strings->content))->len)) == NULL)
			{
				if (res) free(res);
				return (-1);
			}
			if (data->hex)
				print_hex((unsigned char *)res, ft_strlen(res));
			else
				print_res(res, ((t_string *)(data->strings->content))->len, data->o_opt);
			suffix(data, ((t_string *)(data->strings->content))->source_type);
			if (res[((t_string *)(data->strings->content))->len - 1] != '\n')
				printf("\n");
			free(res);
		}
		else
		{
			if (data->hash && (res = data->hash->func_ptr(((t_string *)(data->strings->content))->string, data, &((t_string *)(data->strings->content))->len)) == NULL)
			{
				if (res) free(res);
				return (-1);
			}
			print_res(res, ((t_string *)(data->strings->content))->len, data->o_opt);
			fclose(data->o_opt);
			free(res);
		}
		data->strings = data->strings->next;
	}
	data->strings = tmp_lst;
	if (data->pksiv == 1)
	{
		printf("\nSalt : ");
		print_hex(data->salt, 8);
		printf("\nKey  : ");
		print_hex(data->key, 8);
		printf("\nIV   : ");
		print_hex(data->iv, 8);
		printf("\n");
	}
	return (0);
}
