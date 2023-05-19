/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 11:07:03 by joonhlee          #+#    #+#             */
/*   Updated: 2023/03/17 11:22:44 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	countsigndigit(int n);

void	ft_putnbr_fd(int n, int fd)
{
	char	nbrinstr[12];
	int		i;
	int		sign;

	sign = 1;
	if (n < 0)
	{
		nbrinstr[0] = '-';
		sign = -1;
	}
	i = countsigndigit(n) - 1;
	nbrinstr[i + 1] = '\0';
	while (n / 10 != 0)
	{
		nbrinstr[i--] = sign * (n % 10) + '0';
		n = n / 10;
	}
	nbrinstr[i] = sign * n + '0';
	ft_putstr_fd(nbrinstr, fd);
}

static unsigned int	countsigndigit(int n)
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
