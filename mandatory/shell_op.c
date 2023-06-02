/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 08:37:24 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/02 19:48:23 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	shell_op(char *line, t_env **env_head)
{
	// t_token	*token_head;
	t_cmd	*cmd_head;
	t_here	*here_head;
	int		exit_code;

	// divide line into tokens
	// token_head = token_list_init(line, NULL, NULL, NULL);
	// if (token_head == NULL)
	// 	return (1);
	// test_print_tokens(token_head);
	// create command nodes and rearrange token nodes into sublist of words and redirs
	cmd_head = cmd_list_init(line, NULL, *env_head);
	// if (cmd_head == NULL)
	// 	printf("cmd_head = null\n");
	// if (cmd_head == NULL || g_exit_status != 0)
	// 	return (1);
	// g_exit_status = 0;
	// execute here_docs and replace them into input redirection with heredocs
	// also save here_doc filename as list to unlink in the future as return value
	// if (g_exit_status == 0)
	here_head = NULL;
	if (g_exit_status == 0)
		here_head = repeat_heredocs(cmd_head, *env_head);
	exit_code = 0;
	if (g_exit_status == 0)
		exit_code = exec_cmds(cmd_head, env_head);
		// redirections, execution with forks are performed below
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
		// printf("shell_op:52|cmd %s\n", cmd_iter->words->str);
		if (cmd_iter->next != NULL || cmd_iter->prev != NULL)
			if (manage_pipe(cmd_iter) == -1)
				return (perror_return("minishell: pipe", 1));
		if (cmd_iter->prev == NULL && cmd_iter->next == NULL
			&& cmd_iter->words != NULL
			&& is_builtin(cmd_iter->words->str))
			return (only_builtin_child(cmd_iter, env_head));
		pid = fork();
		if (pid == -1)
			return (perror_return("minishell: fork", 1));
		else if (pid == 0)
			return (child(cmd_iter, env_head));
		else if (pid > 0)
			cmd_iter = cmd_iter->next;
	}
	return (parent(pid, cmd_head));
}

int	manage_pipe(t_cmd *cmd)
{
	int	check;

	if (cmd->prev != NULL && cmd->prev->prev_pfd[0] != -1)
	{
		close(cmd->prev->prev_pfd[0]);
		close(cmd->prev->prev_pfd[1]);
		cmd->prev->prev_pfd[0] = -1;
		cmd->prev->prev->next_pfd[0] = -1;
		cmd->prev->prev_pfd[1] = -1;
		cmd->prev->prev->next_pfd[1] = -1;
	}
	if (cmd->next != NULL)
	{
		check = pipe(cmd->next_pfd);
		if (check == -1)
			return (check);
		cmd->next->prev_pfd[0] = cmd->next_pfd[0];
		cmd->next->prev_pfd[1] = cmd->next_pfd[1];
	}
	return (0);
}

int	only_builtin_child(t_cmd *cmd, t_env **env_head)
{
	int		std_fd[2];
	t_token	*token_iter;
	int		fd;

	cmd->argv = words_lst_to_arr(cmd);
	cmd->cmd_path = ft_strdup(cmd->argv[0]);
	if (cmd->cmd_path == NULL)
		exit (EXIT_FAILURE);
	std_fd[0] = dup(STDIN_FILENO);
	if (std_fd[0] == -1)
		return (perror_return("minishell:", 1));
	std_fd[1] = dup(STDOUT_FILENO);
	if (std_fd[1] == -1)
	{
		close(std_fd[0]);
		return (perror_return("minishell:", 1));
	}
	token_iter = cmd->redirs;
	fd = 0;
	while (token_iter)
	{
		if (token_iter->type == AERROR)
		{
			dup2(std_fd[0], STDIN_FILENO);
			close(std_fd[0]);
			dup2(std_fd[1], STDOUT_FILENO);
			close(std_fd[1]);
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(token_iter->str, STDERR_FILENO);
			ft_putstr_fd(" : ambiguous redirect\n", STDERR_FILENO);
			return (1);
		}
		if (token_iter->type == INFILE)
			fd = open(token_iter->str, O_RDONLY);
		else if (token_iter->type == OUTFILE)
			fd = open(token_iter->str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		else if (token_iter->type == APPEND)
			fd = open(token_iter->str, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (fd == -1)
		{
			dup2(std_fd[0], STDIN_FILENO);
			close(std_fd[0]);
			dup2(std_fd[1], STDOUT_FILENO);
			close(std_fd[1]);
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			return (perror_return(token_iter->str, 1));
		}
		if (token_iter->type == INFILE)
			dup2(fd, STDIN_FILENO);
		else if (token_iter->type == OUTFILE || token_iter->type == APPEND)
			dup2(fd, STDOUT_FILENO);
		close(fd);
		token_iter = token_iter->next;
	}
	run_only_builtin(cmd, env_head);
	dup2(std_fd[0], STDIN_FILENO);
	close(std_fd[0]);
	dup2(std_fd[1], STDOUT_FILENO);
	close(std_fd[1]);
	return (0);
}

int	child(t_cmd *cmd, t_env **env_head)
{
	t_token	*token_iter;
	int		fd;
	char	**envp;

	if (cmd->prev != NULL)
	{
		// ft_putendl_fd("shellop_child:164", STDERR_FILENO);
		close(cmd->prev_pfd[1]);
		dup2(cmd->prev_pfd[0], STDIN_FILENO);
		close(cmd->prev_pfd[0]);
	}
	if (cmd->next != NULL)
	{
		// ft_putendl_fd("shellop_child:171", STDERR_FILENO);
		close(cmd->next_pfd[0]);
		dup2(cmd->next_pfd[1], STDOUT_FILENO);
		close(cmd->next_pfd[1]);
	}
	token_iter = cmd->redirs;
	while (token_iter)
	{
		if (token_iter->type == AERROR)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(token_iter->str, STDERR_FILENO);
			ft_putstr_fd(" : ambiguous redirect\n", STDERR_FILENO);
			exit (1);
		}
		else if (token_iter->type == INFILE)
			fd = open(token_iter->str, O_RDONLY);
		else if (token_iter->type == OUTFILE)
			fd = open(token_iter->str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		else if (token_iter->type == APPEND)
			fd = open(token_iter->str, O_WRONLY | O_APPEND | O_CREAT, 0644);
		// else
		// 	printf("sadlfk");

		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			exit (perror_return(token_iter->str, 1));
		}
		if (token_iter->type == INFILE)
			dup2(fd, STDIN_FILENO);
		else if (token_iter->type == OUTFILE || token_iter->type == APPEND)
			dup2(fd, STDOUT_FILENO);
		close(fd);
		token_iter = token_iter->next;
	}
	cmd->argv = words_lst_to_arr(cmd);
	if (cmd->argv == NULL)
		exit (perror_return("malloc", 1));
	if (cmd->argv[0] == NULL)
		exit (0);
	if (is_builtin(cmd->argv[0]))
		return (run_builtin(cmd, env_head));
	envp = env_conv_arr(*env_head);
	if (envp == 0)
		exit (perror_return("malloc", 1));
	cmd->cmd_path = find_cmd_path(cmd->argv[0], envp);
	// ft_putstr_fd("shell_op:208|cmd_path:", 2);
	// ft_putendl_fd(cmd->cmd_path, 2);
	if (execve(cmd->cmd_path, cmd->argv, envp) == -1)
	{
		// printf("shell_op:203|cmd_path:%s\n", cmd->cmd_path);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->cmd_path, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit (127);
	}
	exit (0);
}

int	parent(int pid, t_cmd *cmd_head)
{
	t_cmd	*cmd_iter;
	int		n_cmd;
	int		status;
	int		wpid;

	n_cmd = 0;
	cmd_iter = cmd_head;
	// printf("shell_op:222|exitstatus:%d\n", g_exit_status);
	while (cmd_iter)
	{
		// printf("shell_op:237|pfd:%d|%d|%d|%d\n", cmd_iter->prev_pfd[0], cmd_iter->prev_pfd[1], cmd_iter->next_pfd[0], cmd_iter->next_pfd[1]);
		// if (cmd_iter->prev_pfd[0] != -1)
		// {
		// 	close(cmd_iter->prev_pfd[0]);
		// 	close(cmd_iter->prev_pfd[1]);
		// }
		if (cmd_iter->next_pfd[0] != -1)
		{
			close(cmd_iter->next_pfd[0]);
			close(cmd_iter->next_pfd[1]);
			cmd_iter->next_pfd[0] = -1;
			cmd_iter->next_pfd[1] = -1;
		}
		n_cmd++;
		cmd_iter = cmd_iter->next;
	}
	cmd_iter = cmd_head;
	// printf("shell_op:238|exitstatus:%d|n_cmd:%d\n", g_exit_status, n_cmd);
	while (n_cmd > 0)
	{
		// printf("shell_op:254|n_cmd:%d\n", n_cmd);
		wpid = wait(&status);
		if (wpid == pid && WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		n_cmd--;
		// printf("shell_op:252|wpid%d\n", wpid);
	}
	// printf("shell_op:243|exitstatus:%d\n", g_exit_status);
	return (g_exit_status);
}

char	**words_lst_to_arr(t_cmd *cmd)
{
	t_token	*token_iter;
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
		exit (EXIT_FAILURE);
	token_iter = cmd->words;
	i = 0;
	while (i < n)
	{
		argv[i] = token_iter->str;
		token_iter = token_iter->next;
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
		free(cmd_to_clear);
	}
	clear_here_n_return(here_head);
}

int	perror_return(char *str, int exit_code)
{
	g_exit_status = exit_code;
	perror(str);
	return (exit_code);
}
