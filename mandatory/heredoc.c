/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 13:32:16 by joonhlee          #+#    #+#             */
/*   Updated: 2023/05/22 13:14:52 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_here	*repeat_heredocs(t_cmd *cmd_head)
{
	t_cmd	*cmd_iter;
	t_token	*redirs_iter;
	t_here	*here_head;
	t_here	*here_doc;

	cmd_iter = cmd_head;
	here_head = NULL;
	while (cmd_iter)
	{
		redirs_iter = cmd_iter->redirs;
		while (redirs_iter)
		{
			if (redirs_iter->type == 5)
			{
				here_doc = do_a_heredoc(redirs_iter->str);
				if (here_doc == 0)
					return (clear_here_n_return(here_head));
				here_add_bottom(&here_head, here_doc);
				update_redirs(redirs_iter, here_doc);
			}
			redirs_iter = redirs_iter->next;
		}
		cmd_iter = cmd_iter->next;
	}
	return (here_head);
}

t_here	*do_a_heredoc(char *limiter)
{
	int		fd;
	char	*here_filename;
	t_here	*heredoc_node;

	here_filename = nexist_name();
	if (here_filename == 0)
		return (NULL);
	fd = open(here_filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd == -1)
		return (free_n_return(here_filename));
	write_heredoc(fd, limiter);
	close(fd);
	heredoc_node = (t_here *) malloc (sizeof (t_here));
	if (heredoc_node == 0)
	{
		unlink(here_filename);
		return (NULL);
	}
	heredoc_node->filename = here_filename;
	heredoc_node->next = NULL;
	return (heredoc_node);
}

char	*nexist_name(void)
{
	int		check;
	char	*ind;
	char	*filename;

	check = 0;
	while (check >= 0)
	{
		ind = ft_itoa(check++);
		if (ind == 0)
			return (0);
		filename = ft_strjoin("heredoc", ind);
		free(ind);
		if (filename == 0)
			return (0);
		if (access(filename, F_OK) != 0)
			return (filename);
		free(filename);
	}
	return (0);
}

void	write_heredoc(int fd, char *limiter_nl)
{
	char	*line;

	while (1)
	{
		line = readline("pipe heredoc> ");
		if (ft_strcmp(line, limiter_nl) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

t_here	*free_n_return(char *str)
{
	free(str);
	return (NULL);
}

t_here	*clear_here_n_return(t_here *here_head)
{
	t_here	*iter;
	t_here	*node_to_clear;

	if (here_head == NULL)
		return (NULL);
	iter = here_head;
	while (iter)
	{
		node_to_clear = iter;
		iter = iter->next;
		unlink(node_to_clear->filename);
		free(node_to_clear->filename);
		free(node_to_clear);
	}
	return (NULL);
}

void	here_add_bottom(t_here **here_head, t_here *here_doc)
{
	t_here	*iter;

	if (*here_head == NULL)
	{
		*here_head = here_doc;
		return ;
	}
	iter = *here_head;
	while (iter->next)
		iter = iter->next;
	iter->next = here_doc;
}

void	update_redirs(t_token *redirs, t_here *here_doc)
{
	free(redirs->str);
	redirs->str = ft_strdup(here_doc->filename);
	if (redirs->str == NULL)
		exit(1);
	redirs->type = INFILE;
}
