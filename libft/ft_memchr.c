/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 17:58:53 by joonhlee          #+#    #+#             */
/*   Updated: 2023/03/17 19:32:29 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*result;

	i = 0;
	result = (unsigned char *) s;
	while (i < n)
	{
		if (*(result + i) == (unsigned char)c)
			break ;
		i++;
	}
	if (i == n)
		return ((void *)0);
	else
		return ((void *)(result + i));
}
