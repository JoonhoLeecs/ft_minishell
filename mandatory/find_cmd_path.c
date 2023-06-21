/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 08:08:57 by joonhlee          #+#    #+#             */
/*   Updated: 2023/06/21 10:27:09 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_cmd_path(char *cmd, char **envp)
{
	char	**dirs;
	char	*cmd_path;

	if (ft_strchr(cmd, '/') != 0)
	{
		cmd_path = ft_strdup(cmd);
		if (cmd_path == NULL)
			exit (EXIT_FAILURE);
		return (cmd_path);
	}
	dirs = find_path_env(envp);
	if (g_exit_status != 0)
	{
		cmd_path = ft_strdup(cmd);
		if (cmd_path == NULL)
			exit (EXIT_FAILURE);
		return (cmd_path);
	}
	cmd_path = check_cmd_path(cmd, dirs);
	free_double_ptr(dirs);
	return (cmd_path);
}

char	**find_path_env(char **envp)
{
	int		i;
	char	**dirs;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i++], "PATH=", 5) == 0)
		{
			dirs = ft_split((envp[i - 1] + 5), ':');
			if (dirs == 0)
				exit (EXIT_FAILURE);
			return (dirs);
		}
	}
	g_exit_status = 127;
	return (NULL);
}

char	*check_cmd_path(char *cmd, char **dirs)
{
	int		i;
	char	*cmd_path;
	char	*slash_cmd;

	slash_cmd = ft_strjoin("/", cmd);
	if (slash_cmd == NULL)
		exit (EXIT_FAILURE);
	i = 0;
	while (dirs[i])
	{
		cmd_path = ft_strjoin(dirs[i++], slash_cmd);
		if (cmd_path == NULL)
			exit (EXIT_FAILURE);
		if (access(cmd_path, F_OK) == 0)
		{
			free(slash_cmd);
			return (cmd_path);
		}
		free (cmd_path);
	}
	free(slash_cmd);
	cmd_path = ft_strdup(cmd);
	if (cmd_path == NULL)
		exit (EXIT_FAILURE);
	return (cmd_path);
}
