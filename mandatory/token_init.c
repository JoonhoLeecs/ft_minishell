/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 14:27:07 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/17 16:41:25 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	check_ingredient(char *split)
{
	t_token_type	type;
	int				len;

	len = ft_strlen(split);
	type = WORD;
	if (ft_strncmp(">>", split, len) == 0)
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
	return (token_head);
}
