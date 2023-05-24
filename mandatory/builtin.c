/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:10:43 by joonhlee          #+#    #+#             */
/*   Updated: 2023/05/24 20:59:38 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (2);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (3);
	else if (ft_strcmp(cmd, "export") == 0)
		return (4);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (5);
	else if (ft_strcmp(cmd, "env") == 0)
		return (6);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (7);
	else
		return (0);
}

int	run_builtin(t_cmd *cmd, t_env **env_head)
{
	int	check;
	// test_print_cmds(cmd);
	check = 0;
	cmd->cmd_path = ft_strdup(cmd->argv[0]);
	if (ft_strcmp(cmd->cmd_path, "echo") == 0)
		check = ft_echo(cmd->argv);
	else if (ft_strcmp(cmd->cmd_path, "cd") == 0)
		check = ft_cd(cmd, *env_head);
	else if (ft_strcmp(cmd->cmd_path, "pwd") == 0)
		check = ft_pwd();
	else if (ft_strcmp(cmd->cmd_path, "export") == 0)
		check = ft_export(cmd, *env_head);
	else if (ft_strcmp(cmd->cmd_path, "unset") == 0)
		check = ft_unset(cmd, env_head);
	else if (ft_strcmp(cmd->cmd_path, "env") == 0)
		check = ft_env(*env_head);
	else if (ft_strcmp(cmd->cmd_path, "exit") == 0)
		check = ft_exit(cmd, 0);
	exit (check);
}

int	ft_echo(char **argv)
{
	int	i;
	int	n_option;

	i = 1;
	n_option = 0;
	if (argv && argv[0] && argv[1] && ft_strcmp(argv[1], "-n") == 0)
	{
		i = 2;
		n_option = 1;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1] != NULL)
			ft_putchar_fd(' ', STDOUT_FILENO);
		else if (argv[i + 1] == NULL && n_option == 0)
			ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
	return (0);
}

int	ft_cd(t_cmd *cmd, t_env *env_head)
{
	int		check;
	char	*old_path;
	char	current_path[PATH_MAX];

	if (cmd->argv[1] == NULL || ft_strlen(cmd->argv[1]) == 0)
		return (0);
	if (ft_strcmp(cmd->argv[1], "-") == 0)
	{
		old_path = env_get_value(env_head, "OLDPWD");
		if (old_path == NULL)
		{
			printf("minishell: cd: OLDPWD not set\n");
			return (1);
		}
		check = chdir(old_path);
		ft_pwd();
	}
	else
		check = chdir(cmd->argv[1]);
	if (check == -1)
	{
		printf("minishell: cd: %s: %s\n", cmd->argv[1], strerror(errno));
		return (1);
	}
	else
	{
		getcwd(current_path, PATH_MAX);
		env_set_value(env_head, "OLDPWD", env_get_value(env_head, "PWD"));
		env_set_value(env_head, "PWD", current_path);
		return (0);
	}
}

int	ft_pwd(void)
{
	char	current_path[PATH_MAX];
	char	*result;

	result = getcwd(current_path, PATH_MAX);
	if (result == NULL)
	{
		perror("minishell:");
		return (errno);
	}
	ft_putendl_fd(current_path, STDOUT_FILENO);
	return (0);
}

int	ft_export(t_cmd *cmd, t_env *env_head)
{
	t_env	*env_iter;
	int		i;
	char	*name;
	char	*value;

	if (cmd->argv[1] == NULL)
	{
		env_iter = env_head;
		while (env_iter)
		{
			printf("declare -x %s", env_iter->name);
			if (env_iter->value != NULL)
				printf("=\"%s\"\n", env_iter->value);
			else
				printf("\n");
			env_iter = env_iter->next;
		}
		return (0);
	}
	i = 1;
	while (cmd->argv[i])
	{
		name = env_find_name(cmd->argv[i]);
		value = env_find_value(cmd->argv[i]);
		env_set_value(env_head, name, value);
		free(name);
		free(value);
		i++;
	}
	return (0);
}

int	ft_unset(t_cmd *cmd, t_env **env_head)
{
	int		i;

	i = 1;
	while (cmd->argv[i])
	{
		env_remove(env_head, cmd->argv[i]);
		i++;
	}
	return (0);
}

int	ft_env(t_env *env_head)
{
	t_env	*env_iter;

	env_iter = env_head;
	while (env_iter)
	{
		if (env_iter->value != NULL)
		{
			ft_putstr_fd(env_iter->name, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putendl_fd(env_iter->value, STDOUT_FILENO);
		}
		env_iter = env_iter->next;
	}
	return (0);
}

int	ft_exit(t_cmd *cmd, int exit_code)
{
	int	i;
	int	n;
	char *a;

	i = 1;
	if (cmd->argv[i] == NULL)
	{
		printf("exit\n");
		exit(exit_code);
	}
	n = ft_atoi(cmd->argv[1]);
	a = ft_itoa(n);
	if (ft_strcmp(cmd->argv[1], a) != 0)
	{
		printf("exit\n");
		printf("bash: exit: %s: numeric argument required\n", cmd->argv[1]);
		free(a);
		exit(255);
	}
	else
	{
		free (a);
		if (cmd->argv[2] != NULL)
		{
			printf("exit\n");
			printf("bash: exit: too many arguments\n");
			return (1);
		}
		else
			exit(n);
	}
}

int	env_set_value(t_env *env_head, char *name, char *value)
{
	t_env	*env_iter;
	t_env	*new;

	env_iter = env_head;
	while (env_iter)
	{
		if (strcmp(env_iter->name, name) == 0)
		{
			free(env_iter->value);
			env_iter->value = ft_strdup(value);
			return (0);
		}
		if (env_iter->next == NULL)
			break ;
		env_iter = env_iter->next;
	}
	name = ft_strdup(name);
	if (value != NULL)
		value = ft_strdup(value);
	new = new_env_node(name, value, env_iter);
	env_iter->next = new;
	return (0);
}

char	*env_get_value(t_env *env_head, char *name)
{
	t_env	*env_iter;

	env_iter = env_head;
	while (env_iter)
	{
		if (strcmp(env_iter->name, name) == 0)
			return (env_iter->value);
		env_iter = env_iter->next;
	}
	return (NULL);
}

int	env_remove(t_env **env_head, char *name)
{
	t_env	*env_iter;
	t_env	*env_to_clear;

	env_to_clear = NULL;
	if (strcmp((*env_head)->name, name) == 0)
	{
		env_to_clear = *env_head;
		*env_head = (*env_head)->next;
		(*env_head)->prev = NULL;
	}
	env_iter = *env_head;
	while (env_iter && env_iter->next)
	{
		if (strcmp(env_iter->next->name, name) == 0)
		{
			env_to_clear = env_iter->next;
			env_iter->next = env_to_clear->next;
			if (env_to_clear->next != NULL)
				env_to_clear->next->prev = env_iter;
			break ;
		}
		env_iter = env_iter->next;
	}
	if (env_to_clear != NULL)
	{
		free(env_to_clear->name);
		free(env_to_clear->value);
		free(env_to_clear);
	}
	return (0);
}
