/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 17:12:35 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/15 20:19:05 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*new_env_node(char *name, char *content, t_env *prev)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	node->name = name;
	node->content = content;
	node->next = NULL;
	node->prev = prev;
	return (node);
}

void	split_name_content(char *envp, char **name, char **content)
{
	int	x;
	int	len;

	x = 0;
	*content = ft_strchr(envp, '=');
	len = *content - envp;
	*content = ft_strdup(*content + 1);
	*name = (char *)malloc(sizeof(char) * (len + 1));
	while (x < len)
	{
		name[0][x] = envp[x];
		x++;
	}
	name[0][x] = 0;
}

t_env	*env_list_init(char **envp, t_env *env_head, t_env *temp, t_env *new)
{
	char	*name;
	char	*content;
	int		i;

	i = 0;
	while (envp[i] != NULL)
	{
		split_name_content(envp[i], &name, &content);
		if (env_head == NULL)
		{
			env_head = new_env_node(name, content, NULL);
			temp = env_head;
		}
		else
		{
			new = new_env_node(name, content, temp);
			temp->next = new;
			temp = new;
		}
		i++;
	}
	return (env_head);
}
