/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 17:12:35 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/19 19:38:06 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*new_env_node(char *name, char *value, t_env *prev)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	node->name = name;
	node->value = value;
	node->next = NULL;
	node->prev = prev;
	return (node);
}

void	split_name_value(char *envp, char **name, char **value)
{
	int	x;
	int	len;

	x = 0;
	*value = ft_strchr(envp, '=');
	len = *value - envp;
	*value = ft_strdup(*value + 1);
	*name = (char *)malloc(sizeof(char) * (len + 1));
	while (x < len)
	{
		name[0][x] = envp[x];
		x++;
	}
	name[0][x] = 0;
}

char	*env_find_name(char *envp)
{
	char	*ptr;
	char	*result;
	int		len;

	ptr = ft_strchr(envp, '=');
	if (ptr == NULL)
	{
		result = ft_strdup(envp);
		return (result);
	}
	len = ptr - envp;
	result = ft_substr(envp, 0, len);
	return (result);
}

char	*env_find_value(char *envp)
{
	char	*ptr;
	char	*result;

	ptr = ft_strchr(envp, '=');
	if (ptr == NULL)
		return (NULL);
	result = ft_strdup(ptr + 1);
	return (result);
}

t_env	*env_list_init(char **envp, t_env *env_head, t_env *temp, t_env *new)
{
	char	*name;
	char	*value;
	int		i;

	i = 0;
	while (envp[i] != NULL)
	{
		split_name_value(envp[i], &name, &value);
		if (env_head == NULL)
		{
			env_head = new_env_node(name, value, NULL);
			temp = env_head;
		}
		else
		{
			new = new_env_node(name, value, temp);
			temp->next = new;
			temp = new;
		}
		i++;
	}
	return (env_head);
}
