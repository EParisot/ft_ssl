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
		printf("%s", ((t_string *)(data->strings->content))->string);
		if (((t_string *)(data->strings->content))->string[ft_strlen(((t_string *)(data->strings->content))->string) - 1] != '\n')
			printf("\n");
	}
	else if (data->r_opt == 0 && data->q_opt == 0)
	{
		if (source_type != _STDIN)
			printf("%s ", data->hash->print_name);
		if (((t_string *)(data->strings->content))->source ||
			((t_string *)(data->strings->content))->string)
		{
			if (source_type == _STDIN)
			{
				printf("(stdin)= ");
			}
			else if (source_type == _FILE)
			{
				printf("(%s)= ", ((t_string *)(data->strings->content))->source);
			}
			else
			{
				if (data->hex)
				{
					printf("(\"");
					print_hex((unsigned char *)((t_string *)(data->strings->content))->string, ft_strlen(((t_string *)(data->strings->content))->string));
					printf("\")= ");
				}
				else
					printf("(\"%s\")= ", ((t_string *)(data->strings->content))->string);
			}
		}
	}
}

static void	suffix(t_data *data, int source_type)
{
	if (data->r_opt == 1 && data->q_opt == 0)
	{
		if (source_type == _STDIN)
			printf(" (stdin)");
		else if (source_type == _FILE)
			printf(" %s", ((t_string *)(data->strings->content))->source);
		else
		{
			if (data->hex)
				{
					printf(" (\"");
					print_hex((unsigned char *)((t_string *)(data->strings->content))->string, ft_strlen(((t_string *)(data->strings->content))->string));
					printf("\")");
				}
				else
					printf(" (\"%s\")", ((t_string *)(data->strings->content))->string);
		}
	}
}

int			hash_string(t_data *data)
{
	t_list	*tmp_lst;
	char 	*res = NULL;

	if (ft_strncmp(data->hash->name, "des", 3) == 0)
		securize(data);
	tmp_lst = data->strings;
	while (data->strings)
	{
		if (data->o_opt == stdout)
		{
			prefix(data, ((t_string *)(data->strings->content))->source_type);
			if (data->hash && (res = data->hash->func_ptr(((t_string *)(data->strings->content))->string, data)) == NULL)
			{
				if (res) free(res);
				return (-1);
			}
			if (data->hex)
				print_hex((unsigned char *)res, ft_strlen(res));
			else
				printf("%s", res);
			free(res);
			suffix(data, ((t_string *)(data->strings->content))->source_type);
			printf("\n");
		}
		else
		{
			if (data->hash && (res = data->hash->func_ptr(((t_string *)(data->strings->content))->string, data)) == NULL)
			{
				if (res) free(res);
				return (-1);
			}
			fprintf(data->o_opt, "%s", res);
			fclose(data->o_opt);
			free(res);
		}
		data->strings = data->strings->next;
	}
	data->strings = tmp_lst;
	return (0);
}
