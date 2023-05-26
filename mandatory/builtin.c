/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:10:43 by joonhlee          #+#    #+#             */
/*   Updated: 2023/05/25 18:42:19 by joonhlee         ###   ########.fr       */
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
	int	j;
	int	n_option;

	i = 1;
	n_option = 0;
	while (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0)
	{
		n_option = i;
		j = 2;
		while (argv[i][j])
		{
			if (argv[i][j] != 'n')
			{
				n_option = 0;
				break ;
			}
			j++;
		}
		if (n_option == 0)
			break ;
		i++;
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
	char	*dest_path;
	char	current_path[PATH_MAX];

	if (cmd->argv[1] != NULL && ft_strlen(cmd->argv[1]) == 0)
		return (0);
	if (cmd->argv[1] == NULL || (cmd->argv[1] && ft_strcmp(cmd->argv[1], "~") == 0))
	{
		dest_path = env_get_value(env_head, "HOME");
		if (dest_path == NULL)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			return (1);
		}
	}
	else if (ft_strcmp(cmd->argv[1], "-") == 0)
	{
		dest_path = env_get_value(env_head, "OLDPWD");
		if (dest_path == NULL)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
			return (1);
		}
	}
	else
	{
		dest_path = ft_strdup(cmd->argv[1]);
		if (dest_path == NULL)
			return (1);
	}
	check = chdir(dest_path);
	if (check == 0 && cmd->argv[1] && ft_strcmp(cmd->argv[1], "-") == 0)
		ft_pwd();
	if (check == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(dest_path);
		free(dest_path);
		return (1);
	}
	else
	{
		getcwd(current_path, PATH_MAX);
		env_set_value(env_head, "OLDPWD", env_get_value(env_head, "PWD"));
		env_set_value(env_head, "PWD", current_path);
		free(dest_path);
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
		return (1);
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
	int		check;

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
	check = 0;
	while (cmd->argv[i])
	{
		name = env_find_name(cmd->argv[i]);
		if (is_valid_name(name) == 0)
		{
			ft_putstr_fd("minishell: export: '", STDERR_FILENO);
			ft_putstr_fd(name, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			free(name);
			check = 1;
			i++;
			continue ;
		}
		value = env_find_value(cmd->argv[i]);
		env_set_value(env_head, name, value);
		free(name);
		free(value);
		i++;
	}
	return (1);
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
		ft_putstr_fd("exit\n", STDERR_FILENO);
		ft_putstr_fd("bash: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd->argv[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		free(a);
		exit(255);
	}
	else
	{
		free (a);
		if (cmd->argv[2] != NULL)
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			ft_putstr_fd("bash: exit: too many arguments\n", STDERR_FILENO);
			return (1);
		}
		else
			exit(n % 256);
	}
	return (1);
}

int	is_valid_name(char *str)
{
	int	i;

	if (str[0] == '\0' || !(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
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
			if (value != NULL)
				env_iter->value = ft_strdup(value);
			else
				env_iter->value = NULL;
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
	char	*result;

	env_iter = env_head;
	while (env_iter)
	{
		if (strcmp(env_iter->name, name) == 0)
		{
			if (env_iter->value == NULL)
				return (NULL);
			result = ft_strdup(env_iter->value);
			return (result);
		}
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
