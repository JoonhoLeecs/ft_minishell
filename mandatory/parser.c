/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 09:22:28 by joonhlee          #+#    #+#             */
/*   Updated: 2023/05/22 11:54:39 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parser(t_token *token_head, t_env *env_head)
{
	t_cmd	*cmd_head;
	t_token	*token_iter;
	t_cmd	*cmd_iter;
	t_env	*env_iter;

	env_iter = env_head;
	cmd_head = new_cmd_node();
	token_iter = token_head;
	cmd_iter = cmd_head;
	while (token_iter)
	{
		if (token_iter->type == PIPE)
		{
			cmd_iter->next = new_cmd_node();
			cmd_iter->next->prev = cmd_iter;
			cmd_iter = cmd_iter->next;
			token_head = token_iter;
			token_iter = token_iter->next;
			free(token_head->str);
			free(token_head);
		}
		else if (token_iter->type == WORD)
		{
			if (cmd_iter->words == NULL)
			{
				cmd_iter->words = token_iter;
				cmd_iter->words_tail = token_iter;
				token_iter->prev = NULL;
				token_iter = token_iter->next;
				cmd_iter->words_tail->next = NULL;
			}
			else
			{
				cmd_iter->words_tail->next = token_iter;
				token_iter->prev = cmd_iter->words_tail;
				cmd_iter->words_tail = token_iter;
				token_iter = token_iter->next;
				cmd_iter->words_tail->next = NULL;
			}
		}
		else if (token_iter->type > REDIR)
		{
			if (cmd_iter->redirs == NULL)
			{
				cmd_iter->redirs = token_iter;
				cmd_iter->redirs_tail = token_iter;
				token_iter->prev = NULL;
				token_iter = token_iter->next;
				cmd_iter->redirs_tail->next = NULL;
			}
			else
			{
				cmd_iter->redirs_tail->next = token_iter;
				token_iter->prev = cmd_iter->redirs_tail;
				cmd_iter->redirs_tail = token_iter;
				token_iter = token_iter->next;
				cmd_iter->redirs_tail->next = NULL;
			}
		}
	}
	return (cmd_head);
}

t_cmd	*new_cmd_node(void)
{
	t_cmd	*node;

	node = (t_cmd *)malloc(sizeof(t_cmd));
	node->words = NULL;
	node->words_tail = NULL;
	node->redirs = NULL;
	node->redirs_tail = NULL;
	node->prev_pfd[0] = -1;
	node->prev_pfd[1] = -1;
	node->next_pfd[0] = -1;
	node->next_pfd[1] = -1;
	node->cmd_path = NULL;
	node->argv = NULL;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}
