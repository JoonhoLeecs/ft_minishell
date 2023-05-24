/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 08:37:24 by joonhlee          #+#    #+#             */
/*   Updated: 2023/05/24 20:55:53 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	shell_op(char *line, t_env **env_head)
{
	t_token	*token_head;
	t_cmd	*cmd_head;
	t_here	*here_head;
	int		exit_code;

	// divide line into tokens
	token_head = token_list_init(line, NULL, NULL, NULL);
	if (token_head == NULL)
		return (1);
	// test_print_tokens(token_head);
	// create command nodes and rearrange token nodes into sublist of words and redirs
	cmd_head = parser(token_head, *env_head);
	if (cmd_head == NULL)
		return (1);
	// execute here_docs and replace them into input redirection with heredocs
	// also save here_doc filename as list to unlink in the future as return value
	here_head = repeat_heredocs(cmd_head);
	// if (here_head == NULL)
	// 	return (1);
	// redirections, execution with forks are performed below
	exit_code = exec_cmds(cmd_head, env_head);
	clear_this_line(cmd_head, here_head);
	return (exit_code);
}

int	exec_cmds(t_cmd *cmd_head, t_env **env_head)
{
	t_cmd	*cmd_iter;
	int		pid;

	cmd_iter = cmd_head;
	while (cmd_iter)
	{
		if (cmd_iter->next != NULL)
			if (open_pipe(cmd_iter) == -1)
				return (perror_return("pipe", 1));
		if (cmd_iter->prev == NULL && cmd_iter->next == NULL
			&& (ft_strcmp(cmd_iter->words->str, "cd") == 0))
		{
			cmd_iter->argv = words_lst_to_arr(cmd_iter);
			cmd_iter->cmd_path = ft_strdup(cmd_iter->argv[0]);
			return (ft_cd(cmd_iter, *env_head));
		}
		if (cmd_iter->prev == NULL && cmd_iter->next == NULL
			&& (ft_strcmp(cmd_iter->words->str, "unset") == 0))
		{
			cmd_iter->argv = words_lst_to_arr(cmd_iter);
			cmd_iter->cmd_path = ft_strdup(cmd_iter->argv[0]);
			return (ft_unset(cmd_iter, env_head));
		}
		if (cmd_iter->prev == NULL && cmd_iter->next == NULL
			&& (ft_strcmp(cmd_iter->words->str, "export") == 0))
		{
			cmd_iter->argv = words_lst_to_arr(cmd_iter);
			cmd_iter->cmd_path = ft_strdup(cmd_iter->argv[0]);
			return (ft_export(cmd_iter, *env_head));
		}
		if (cmd_iter->prev == NULL && cmd_iter->next == NULL
			&& (ft_strcmp(cmd_iter->words->str, "exit") == 0))
		{
			cmd_iter->argv = words_lst_to_arr(cmd_iter);
			cmd_iter->cmd_path = ft_strdup(cmd_iter->argv[0]);
			return (ft_exit(cmd_iter, 0));
		}
		pid = fork();
		if (pid == -1)
			return (perror_return("fork", 1));
		else if (pid == 0)
			return (child(cmd_iter, env_head));
		else if (pid > 0)
			cmd_iter = cmd_iter->next;
	}
	return (parent(pid, cmd_head));
}

int	open_pipe(t_cmd *cmd)
{
	int	check;

	check = pipe(cmd->next_pfd);
	if (check == -1)
		return (check);
	cmd->next->prev_pfd[0] = cmd->next_pfd[0];
	cmd->next->prev_pfd[1] = cmd->next_pfd[1];
	return (0);
}

int	child(t_cmd *cmd, t_env **env_head)
{
	t_token	*token_iter;
	int		fd;
	char	**envp;

	if (cmd->prev != NULL)
	{
		close(cmd->prev_pfd[1]);
		dup2(cmd->prev_pfd[0], STDIN_FILENO);
		close(cmd->prev_pfd[0]);
	}
	if (cmd->next != NULL)
	{
		close(cmd->next_pfd[0]);
		dup2(cmd->next_pfd[1], STDOUT_FILENO);
		close(cmd->next_pfd[1]);
	}
	token_iter = cmd->redirs;
	while (token_iter)
	{
		if (token_iter->type == INFILE)
		{
			fd = open(token_iter->str, O_RDONLY);
			if (fd == -1)
				return (perror_return("open:", 1));
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (token_iter->type == OUTFILE)
		{
			fd = open(token_iter->str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if (fd == -1)
				return (perror_return("open:", 1));
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (token_iter->type == APPEND)
		{
			fd = open(token_iter->str, O_WRONLY | O_APPEND | O_CREAT, 0644);
			if (fd == -1)
				return (perror_return("open:", 1));
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		token_iter = token_iter->next;
	}
	cmd->argv = words_lst_to_arr(cmd);
	if (cmd->argv == 0)
		return (perror_return("malloc", 1));
	if (is_builtin(cmd->argv[0]))
		return (run_builtin(cmd, env_head));
	envp = env_conv_arr(*env_head);
	if (envp == 0)
		return (perror_return("malloc", 1));
	cmd->cmd_path = find_cmd_path(cmd->argv[0], envp);
	if (cmd->cmd_path == 0)
		return (perror_return("malloc", 1));
	if (execve(cmd->cmd_path, cmd->argv, envp) == -1)
	{
		free_double_ptr(cmd->argv);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		perror("cmd");
		return (127);
	}
	return (0);
}

int	parent(int pid, t_cmd *cmd_head)
{
	t_cmd	*cmd_iter;
	int		n_cmd;
	int		status;
	int		wpid;
	int		exit_code;

	n_cmd = 0;
	cmd_iter = cmd_head;
	while (cmd_iter)
	{
		close(cmd_iter->prev_pfd[0]);
		close(cmd_iter->prev_pfd[1]);
		close(cmd_iter->next_pfd[0]);
		close(cmd_iter->next_pfd[1]);
		n_cmd++;
		cmd_iter = cmd_iter->next;
	}
	while (n_cmd-- > 0)
	{
		wpid = wait(&status);
		if (wpid == pid && WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
	}
	return (exit_code);
}

char	**words_lst_to_arr(t_cmd *cmd)
{
	t_token	*token_iter;
	// t_token	*token_tmp;
	int		n;
	char	**argv;
	int		i;

	token_iter = cmd->words;
	n = 0;
	while (token_iter)
	{
		n++;
		token_iter = token_iter->next;
	}
	argv = (char **)malloc(sizeof(char *) * (n + 1));
	if (argv == NULL)
		return (NULL);
	token_iter = cmd->words;
	i = 0;
	while (i < n)
	{
		argv[i] = token_iter->str;
		// token_tmp = token_iter;
		token_iter = token_iter->next;
		// free(token_tmp);
		i++;
	}
	argv[i] = NULL;
	return (argv);
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
		if (cmd_to_clear->cmd_path != NULL)
			free(cmd_to_clear->cmd_path);
		if (cmd_to_clear->argv != NULL)
			free(cmd_to_clear->argv);
	}
	here_iter = here_head;
	while (here_iter)
	{
		unlink(here_iter->filename);
		here_to_clear = here_iter;
		here_iter = here_iter->next;
		free(here_to_clear->filename);
		free(here_to_clear);
	}
}

int	perror_return(char *str, int exit_code)
{
	perror(str);
	return (exit_code);
}
