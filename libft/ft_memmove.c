/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 14:45:39 by joonhlee          #+#    #+#             */
/*   Updated: 2023/03/15 17:45:23 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
