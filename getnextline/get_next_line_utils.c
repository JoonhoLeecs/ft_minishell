/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 14:43:27 by joonhlee          #+#    #+#             */
/*   Updated: 2023/04/24 11:23:42 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_remains	*ft_find_create_node(t_remains **head, int fd)
{
	t_remains	*fd_node;

	fd_node = *head;
	while (fd_node)
	{
		if (fd_node->fd == fd)
			return (fd_node);
		fd_node = fd_node->next;
	}
	fd_node = (t_remains *) malloc(sizeof(t_remains));
	if (fd_node == 0)
		return (0);
	fd_node->fd = fd;
	fd_node->fd_remains = (char *) 0;
	fd_node->next = *head;
	*head = fd_node;
	return (fd_node);
}

size_t	ft_find_nchr_to_cpy(char *buffer, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		if (buffer[i] == '\n')
			return (i + 1);
		i++;
	}
	return (i);
}

size_t	ft_strlen_gnl(const char *s)
{
	size_t	i;

	if (s == 0)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t			i;
	unsigned char	*ptr_src;
	unsigned char	*ptr_dst;

	i = 0;
	ptr_src = (unsigned char *)src;
	ptr_dst = (unsigned char *)dst;
	if (dst == src)
		return (dst);
	while (i < len)
	{
		if (dst > src)
			*(ptr_dst + len - 1 - i) = *(ptr_src + len - 1 - i);
		else
			*(ptr_dst + i) = *(ptr_src + i);
		i++;
	}
	return ((void *)ptr_dst);
}

void	ft_clear_fd_node(t_remains **head, int fd)
{
	t_remains	*prev_node;
	t_remains	*node_to_clear;

	prev_node = *head;
	if (prev_node->fd == fd)
	{
		node_to_clear = prev_node;
		*head = prev_node->next;
	}
	while (prev_node->next)
	{
		if (prev_node->next->fd == fd)
		{
			node_to_clear = prev_node->next;
			prev_node->next = prev_node->next->next;
			break ;
		}
		prev_node = prev_node->next;
	}
	free(node_to_clear->fd_remains);
	node_to_clear->fd_remains = 0;
	node_to_clear->next = 0;
	node_to_clear->fd = 0;
	free(node_to_clear);
	node_to_clear = 0;
}
