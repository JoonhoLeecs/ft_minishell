/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 11:47:04 by joonhlee          #+#    #+#             */
/*   Updated: 2023/03/22 13:02:40 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	nbr;

	i = 0;
	sign = 1;
	nbr = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '+' || str[i] == '-')
		sign = 44 - str[i++];
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (sign > 0 && ((nbr > LONG_MAX / 10) || (nbr == LONG_MAX / 10
					&& str[i] - '0' > LONG_MAX % 10)))
			return ((int)LONG_MAX);
		if (sign < 0 && ((nbr < LONG_MIN / 10) || (nbr == LONG_MIN / 10
					&& sign * (str[i] - '0') < LONG_MIN % 10)))
			return ((int)LONG_MIN);
		nbr = nbr * 10 + sign * (str[i++] - '0');
	}
	return ((int)nbr);
}

//to be updated if overflow error should be returned
