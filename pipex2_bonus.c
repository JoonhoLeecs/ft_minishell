/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 08:08:57 by joonhlee          #+#    #+#             */
/*   Updated: 2023/05/05 10:11:53 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*find_cmd_path(char *cmd, char **envp)
{
	char	**dirs;
	char	*cmd_path;

	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	cmd = ft_strjoin("/", cmd);
	if (cmd == 0)
		return (0);
	dirs = find_path_env(envp, &cmd);
	if (dirs == 0)
		return (0);
	cmd_path = check_cmd_path(cmd, dirs);
	free (cmd);
	free_double_ptr(dirs);
	return (cmd_path);
}

char	**find_path_env(char **envp, char **cmd)
{
	int		i;
	char	**dirs;

	i = 0;
	while (envp[i])
		if (ft_strncmp(envp[i++], "PATH=", 5) == 0)
			break ;
	dirs = ft_split((envp[i - 1] + 5), ':');
	if (dirs == 0)
	{
		free(*cmd);
		*cmd = 0;
		return (0);
	}
	return (dirs);
}

char	*check_cmd_path(char *cmd, char **dirs)
{
	int		i;
	char	*cmd_path;

	i = 0;
	while (dirs[i])
	{
		cmd_path = ft_strjoin(dirs[i++], cmd);
		if (cmd_path == 0)
			return (0);
		if (access(cmd_path, F_OK) == 0)
			return (cmd_path);
		free (cmd_path);
	}
	cmd_path = ft_strjoin(".", cmd);
	if (cmd_path == 0)
		return (0);
	return (cmd_path);
}

void	free_double_ptr(char **ptr)
{
	int	i;

	if (ptr == 0)
		return ;
	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free (ptr);
}
