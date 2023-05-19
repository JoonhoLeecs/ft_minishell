/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:23:27 by joonhlee          #+#    #+#             */
/*   Updated: 2023/03/14 16:54:15 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, char *src, size_t size)
{
	size_t	src_len;
	size_t	dest_len;
	size_t	src_index;
	size_t	dest_index;
	size_t	ret_len;

	src_len = ft_strlen(src);
	dest_len = ft_strlen(dest);
	if (size < dest_len)
		ret_len = size + src_len;
	else
		ret_len = dest_len + src_len;
	if (size == 0)
		return (ret_len);
	src_index = 0;
	dest_index = dest_len;
	while (src[src_index] != '\0' && size > dest_len + src_len)
		dest[dest_index++] = src[src_index++];
	while (dest_index + 1 < size && size <= dest_len + src_len)
		dest[dest_index++] = src[src_index++];
	dest[dest_index] = '\0';
	return (ret_len);
}
