/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 13:59:12 by joonhlee          #+#    #+#             */
/*   Updated: 2023/03/22 13:12:27 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	count_sign_digit(int n);

char	*ft_itoa(int n)
{
	char	*result;
	int		i;
	int		sign;

	result = (char *) malloc((count_sign_digit(n) + 1) * sizeof(char));
	if (result == 0)
		return ((char *)0);
	sign = 1;
	if (n < 0)
	{
		result[0] = '-';
		sign = -1;
	}
	i = count_sign_digit(n) - 1;
	result[i + 1] = '\0';
	while (n / 10 != 0)
	{
		result[i--] = sign * (n % 10) + '0';
		n = n / 10;
	}
	result[i] = sign * n + '0';
	return (result);
}

static unsigned int	count_sign_digit(int n)
{
	unsigned int	i;

	i = 1;
	if (n < 0)
		i++;
	while (n / 10 != 0)
	{
		i++;
		n = n / 10;
	}
	return (i);
}
