/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 08:37:24 by joonhlee          #+#    #+#             */
/*   Updated: 2023/05/19 17:38:04 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	shell_op(char *line, t_env *env_head)
{
	t_token	*token_head;
	t_cmd	*cmd_head;
	t_here	*here_head;
	int		exit_code;

	// divide line into tokens
	token_head = token_list_init(line, NULL, NULL, NULL);
	if (token_head == NULL)
		return (1);
	// create command nodes and rearrange token nodes into sublist of words and redirs
	cmd_head = parser(token_head, env_head);
	if (cmd_head == NULL)
		return (1);
	// execute here_docs and replace them into input redirection with heredocs
	// also save here_doc filename as list to unlink in the future as return value
	here_head = repeat_heredocs(cmd_head);
	if (here_head == 0)
		return (1);
	// redirections, execution with forks are performed below
	exit_code = exec_cmds(cmd_head, env_head);
	clear_this_line(cmd_head, here_head);
	return (exit_code);
}

int	exec_cmds(t_cmd *cmd_head, t_env *env_head)
{
	t_cmd	*cmd_iter;
	int		check;
	int		pid;

	cmd_iter = cmd_head;
	while (cmd_iter)
	{
		if (cmd_iter->next != NULL)
			if (open_pipe(cmd_iter) == -1)
				return (perror_n_return("pipe", 1));
		pid = fork();
		if (pid == -1)
			return (perror_n_return("fork", 1));
		else if (pid == 0)
			return (child(cmd_iter, env_head));
		else if (pid > 0)
			cmd_iter = cmd_iter->next;
	}
	return (parent(pid, cmd_head));
}

void	clear_this_line(t_cmd *cmd_head, t_here *here_head)
{
	t_cmd	*cmd_iter;
	t_cmd	*cmd_to_clear;
	t_token	*token_iter;
	t_token	*token_to_clear;
	t_here	*here_iter;
	t_here	*here_to_clear;

	cmd_iter = cmd_head;
	while (cmd_iter)
	{
		cmd_to_clear = cmd_iter;
		cmd_iter = cmd_iter->next;
		token_iter = cmd_to_clear->words;
		while (token_iter)
		{
			token_to_clear = token_iter;
			token_iter = token_iter->next;
			free(token_to_clear->str);
			free(token_to_clear);
		}
		token_iter = cmd_to_clear->redirs;
		while (token_iter)
		{
			token_to_clear = token_iter;
			token_iter = token_iter->next;
			free(token_to_clear->str);
			free(token_to_clear);
		}
		free(cmd_to_clear->cmd_path);
		free(cmd_to_clear->argv);
	}
	here_iter = here_head;
	while (here_iter)
	{
		here_to_clear = here_iter;
		here_iter = here_iter->next;
		free(here_to_clear->filename);
		free(here_to_clear);

	}
}
