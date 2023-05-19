/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 19:28:39 by joonhlee          #+#    #+#             */
/*   Updated: 2023/03/16 12:18:40 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	result_len;
	char	*result;
	size_t	i;

	if (ft_strlen(s) > start + len)
		result_len = len;
	else if (ft_strlen(s) < start)
		result_len = 0;
	else
		result_len = ft_strlen(s) - start;
	result = (char *) malloc((result_len + 1) * sizeof (char));
	if (result == 0)
		return ((char *)0);
	i = 0;
	while (i < result_len)
	{
		result[i] = s[start + i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
