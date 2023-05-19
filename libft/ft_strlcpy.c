/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:12:17 by joonhlee          #+#    #+#             */
/*   Updated: 2023/03/14 16:26:46 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	srclength;

	i = 0;
	srclength = ft_strlen(src);
	if (dstsize == 0)
		return (srclength);
	while (src[i] != '\0' && dstsize > srclength)
	{
		*(dst + i) = *(src + i);
		i++;
	}
	while (i < dstsize - 1 && dstsize <= srclength)
	{
		*(dst + i) = *(src + i);
		i++;
	}
	*(dst + i) = '\0';
	return (srclength);
}
