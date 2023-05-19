/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 20:29:35 by joonhlee          #+#    #+#             */
/*   Updated: 2023/03/22 14:45:02 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*result;
	t_list	*new;
	void	*content;

	result = 0;
	if (f == 0 || del == 0)
		return (result);
	while (lst)
	{
		content = f(lst->content);
		new = ft_lstnew(content);
		if (new == 0)
		{
			del(content);
			ft_lstclear(&result, del);
			result = 0;
			break ;
		}
		ft_lstadd_back(&result, new);
		lst = lst->next;
	}
	return (result);
}
