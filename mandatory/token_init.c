/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 14:27:07 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/22 11:43:51 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	check_ingredient(char *split)
{
	t_token_type	type;
	int				len;

	len = ft_strlen(split);
	type = WORD;
	if (ft_strncmp("|", split, len) == 0)
		return (PIPE);
	else if (ft_strncmp(">", split, len) == 0
		|| ft_strncmp(">>", split, len) == 0
		|| ft_strncmp("<", split, len) == 0
		|| ft_strncmp("<<", split, len) == 0)
		return (REDIR);
	return (type);
}

t_token	*new_token_node(t_token_type type, char	*split, t_token *prev)
{
	t_token	*node;

	node = (t_token *)malloc(sizeof(t_token));
	node->type = type;
	node->str = ft_strdup(split);
	node->next = NULL;
	node->prev = prev;
	return (node);
}

t_token	*token_list_init(char *str, t_token *token_head, \
						t_token *temp, t_token *new)
{
	char			**split;
	t_token_type	type;
	int				i;

	split = ft_split(str, ' ');
	i = 0;
	while (split[i] != NULL)
	{
		type = check_ingredient(split[i]);
		if (token_head == NULL)
		{
			token_head = new_token_node(type, split[i], NULL);
			temp = token_head;
		}
		else
		{
			new = new_token_node(type, split[i], temp);
			temp->next = new;
			temp = new;
		}
		i++;
	}
	arr2d_free(split);
	merge_redir(&token_head);
	return (token_head);
}

void	merge_redir(t_token **token_head)
{
	t_token	*token_iter;
	t_token	*new;

	token_iter = *token_head;
	if (token_iter && token_iter->next
		&& (token_iter->type == REDIR && token_iter->next->type == WORD))
	{
		new = new_token_node(convert_type(token_iter->str),
				token_iter->next->str, NULL);
		*token_head = new;
		new->next = token_iter->next->next;
		token_iter->next->next->prev = new;
		free(token_iter->next->str);
		free(token_iter->next);
		free(token_iter->str);
		free(token_iter);
		token_iter = new->next;
	}
	while (token_iter && token_iter->next)
	{
		if (token_iter->type == REDIR && token_iter->next->type == WORD)
		{
			new = new_token_node(convert_type(token_iter->str),
					token_iter->next->str, NULL);
			token_iter->prev->next = new;
			new->prev = token_iter->prev;
			new->next = token_iter->next->next;
			if (token_iter->next->next)
				token_iter->next->next->prev = new;
			free(token_iter->next->str);
			free(token_iter->next);
			free(token_iter->str);
			free(token_iter);
			token_iter = new->next;
		}
		else
			token_iter = token_iter->next;
	}
}

t_token_type	convert_type(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (ft_strncmp(str, "<", len) == 0)
		return (INFILE);
	else if (ft_strncmp(str, ">", len) == 0)
		return (OUTFILE);
	else if (ft_strncmp(str, "<<", len) == 0)
		return (HEREDOC);
	else
		return (APPEND);
}
