/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 19:34:32 by joonhlee          #+#    #+#             */
/*   Updated: 2023/03/22 14:54:50 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp_next;

	if (lst == 0 || *lst == 0 || del == 0)
	{
		return ;
	}
	while (*lst)
	{
		tmp_next = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = tmp_next;
	}
}
