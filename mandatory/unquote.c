/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unquote.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 16:21:59 by joonhlee          #+#    #+#             */
/*   Updated: 2023/05/31 17:09:39 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unquote(t_token *token_head)
{
	t_token	*token_iter;
	char	*unquoted_str;

	token_iter = token_head;
	while (token_iter)
	{
		unquoted_str = unquote_a_str(token_iter->str);
		free(token_iter->str);
		token_iter->str = unquoted_str;
		token_iter = token_iter->next;
	}
	return (0);
}

char	*unquote_a_str(char *str)
{
	t_quotes	q;
	char		*result;
	int			len;
	int			str_idx;
	int			res_idx;

	len = ft_strlen(str) - find_n_of_quotes(str) + 1;
	result = (char *)malloc(len * sizeof (char));
	if (result == NULL)
		exit (1);
	q.detect = 0;
	str_idx = 0;
	res_idx = 0;
	while (str[str_idx])
	{
		if (check_quotes2(str[str_idx], &q) == -1)
		{
			result[res_idx] = str[str_idx];
			res_idx++;
		}
		str_idx++;
	}
	result[res_idx] = '\0';
	return (result);
}

int	find_n_of_quotes(char *str)
{
	t_quotes	q;
	int			sum;
	int			i;

	q.detect = 0;
	sum = 0;
	i = 0;
	while (str[i])
	{
		if (check_quotes2(str[i], &q) > -1)
			sum++;
		i++;
	}
	return (sum);
}

int	check_quotes2(char s, t_quotes *q)
{
	if (q->detect == 0 && (s == '\'' || s == '\"'))
	{
		q->detect = 1;
		q->type = s;
		return (0);
	}
	else if (q->detect == 1 && (s == q->type))
	{
		q->detect = 0;
		return (1);
	}
	else
		return (-1);
}
