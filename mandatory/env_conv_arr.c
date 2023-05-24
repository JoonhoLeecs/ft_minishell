/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_conv_arr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 19:50:32 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/23 20:24:35 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	input_name_value(t_env *env_head, char *env_arr, int size)
{
	int		i;
	char	*name_equal;
	char	*str_total;

	name_equal = ft_strjoin(env_head->name, "=");
	str_total = ft_strjoin(name_equal, env_head->value);
	i = 0;
	while (i < size - 1)
	{
		env_arr[i] = str_total[i];
		i++;
	}
	env_arr[i] = 0;
	free(name_equal);
	free(str_total);
}

char	**env_conv_arr(t_env *env_head)
{
	int		i;
	int		size;
	char	**env_arr;
	t_env	*temp;

	size = 0;
	temp = env_head;
	while (temp != NULL)
	{
		temp = temp->next;
		size++;
	}
	env_arr = (char **)malloc(sizeof(char *) * (size + 1));
	i = 0;
	while (env_head != NULL)
	{
		size = ft_strlen(env_head->name) + ft_strlen(env_head->value) + 2;
		env_arr[i] = (char *)malloc(sizeof(char) * size);
		input_name_value(env_head, env_arr[i], size);
		env_head = env_head->next;
		i++;
	}
	env_arr[i] = 0;
	return (env_arr);
}
