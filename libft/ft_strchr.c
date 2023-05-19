/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:56:26 by joonhlee          #+#    #+#             */
/*   Updated: 2023/03/17 13:54:19 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t			i;
	size_t			len;
	size_t			has_c;
	unsigned char	uc;
	char			*result;

	i = 0;
	len = ft_strlen(s);
	has_c = 0;
	uc = (unsigned char) c;
	while ((i < len && uc != 0) || (i <= len && uc == 0))
	{
		if ((unsigned char)s[i] == uc)
		{
			has_c = 1;
			result = (char *)(s + i);
			break ;
		}
		i++;
	}
	if (has_c == 0)
		return ((char *)0);
	else
		return (result);
}
