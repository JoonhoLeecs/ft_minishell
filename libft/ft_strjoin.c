/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 12:17:01 by joonhlee          #+#    #+#             */
/*   Updated: 2023/03/17 11:46:03 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	size_t	result_len;
	size_t	s1_len;

	s1_len = ft_strlen(s1);
	result_len = s1_len + ft_strlen(s2);
	result = (char *) malloc((result_len + 1) * sizeof(char));
	if (result == 0)
		return ((char *)0);
	ft_memmove(result, s1, s1_len * sizeof (char));
	ft_memmove((result + s1_len), s2, (result_len - s1_len) * sizeof (char));
	result[result_len] = '\0';
	return (result);
}
