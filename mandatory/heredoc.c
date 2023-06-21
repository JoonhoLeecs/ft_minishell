/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 13:32:16 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/21 10:32:23 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_here	*repeat_heredocs(t_cmd *cmd_head, t_env *env_head)
{
	t_here	*here_head;
	int		stdin_fd;

	stdin_fd = dup(STDIN_FILENO);
	if (stdin_fd == -1)
	{
		perror("minishell");
		return (NULL);
	}
	here_signal_setup();
	here_head = here_iter(cmd_head, env_head);
	signal_setup();
	if (g_exit_status < 0)
	{
		g_exit_status = 1;
		dup2(stdin_fd, STDIN_FILENO);
	}
	close(stdin_fd);
	return (here_head);
}

t_here	*here_iter(t_cmd *cmd_head, t_env *env_head)
{
	t_cmd	*cmd_iter;
	t_token	*redirs_iter;
	t_here	*here_head;
	t_here	*here_node;

	here_head = NULL;
	cmd_iter = cmd_head;
	while (cmd_iter)
	{
		redirs_iter = cmd_iter->redirs;
		while (redirs_iter && g_exit_status > -1)
		{
			if (redirs_iter->type == HEREDOC)
			{
				here_node = do_a_heredoc(redirs_iter->str, env_head);
				if (here_node == NULL)
					return (clear_here_n_return(here_head));
				here_add_bottom(&here_head, here_node);
				update_redirs(redirs_iter, here_node);
			}
			redirs_iter = redirs_iter->next;
		}
		cmd_iter = cmd_iter->next;
	}
	return (here_head);
}

t_here	*do_a_heredoc(char *limiter, t_env *env_head)
{
	int		fd;
	char	*here_filename;
	t_here	*heredoc_node;

	here_filename = nexist_name();
	if (here_filename == NULL)
	{
		g_exit_status = 1;
		return (NULL);
	}
	fd = open(here_filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd == -1)
		return (free_n_return(here_filename, 1));
	write_heredoc(fd, limiter, env_head);
	close(fd);
	heredoc_node = (t_here *) malloc (sizeof (t_here));
	if (heredoc_node == NULL)
	{
		unlink(here_filename);
		exit (EXIT_FAILURE);
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
			exit (EXIT_FAILURE);
		filename = ft_strjoin("heredoc", ind);
		free(ind);
		if (filename == 0)
			exit (EXIT_FAILURE);
		if (access(filename, F_OK) != 0)
			return (filename);
		free(filename);
	}
	return (NULL);
}

void	write_heredoc(int fd, char *limiter, t_env *env_head)
{
	char	*line;
	char	*expanded_line;

	while (1 && g_exit_status > -1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (line != NULL && *line != '\0' && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (g_exit_status < 0 || line == NULL || ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		expanded_line = exp_str_to_str(line, env_head);
		ft_putendl_fd(expanded_line, fd);
		free(expanded_line);
		free(line);
	}
}
