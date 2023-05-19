/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: woosekim <woosekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 19:22:54 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/17 14:46:17 by woosekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	arr2d_free(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	list_free(t_env *head)
{
	t_env	*temp;

	temp = head;
	while (temp != NULL)
	{
		temp = head->next;
		free(head->name);
		free(head->content);
		free(head);
		head = temp;
	}
}
