/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 18:54:12 by joonhlee          #+#    #+#             */
/*   Updated: 2023/03/15 17:59:04 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	haystack_ind;
	size_t	needle_ind;
	size_t	needle_len;

	needle_len = ft_strlen(needle);
	if (needle_len == 0)
		return ((char *)haystack);
	haystack_ind = 0;
	while (haystack_ind < len && haystack[haystack_ind])
	{
		needle_ind = 0;
		while (haystack_ind + needle_ind < len && needle[needle_ind])
		{
			if (haystack[haystack_ind + needle_ind] != needle[needle_ind])
				break ;
			needle_ind++;
		}
		if (needle_ind == needle_len)
			return ((char *)(haystack + haystack_ind));
		haystack_ind++;
	}
	return (0);
}
