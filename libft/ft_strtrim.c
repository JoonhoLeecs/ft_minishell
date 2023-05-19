/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 12:48:28 by joonhlee          #+#    #+#             */
/*   Updated: 2023/03/22 10:28:27 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	isset(char c, char const *set);
static int	find_start_of_trimmed(char const *s1, char const *set);
static int	find_end_of_trimmed(char const *s1, char const *set);

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start_ind;
	int		end_ind;
	char	*result;
	int		s1_ind;
	int		result_ind;

	start_ind = find_start_of_trimmed(s1, set);
	end_ind = find_end_of_trimmed(s1, set);
	if (start_ind > end_ind)
		start_ind = end_ind--;
	result = (char *)malloc((end_ind - start_ind + 2) * sizeof(char));
	if (result == 0)
		return ((char *)0);
	s1_ind = start_ind;
	result_ind = 0;
	while (s1_ind <= end_ind)
	{
		result[result_ind++] = s1[s1_ind];
		s1_ind++;
	}
	result[result_ind] = '\0';
	return (result);
}

static int	find_start_of_trimmed(char const *s1, char const *set)
{
	int	setcheck;
	int	i;

	i = 0;
	setcheck = 1;
	while (s1[i])
	{
		if (isset(s1[i], set) - setcheck == -1)
			break ;
		setcheck = isset(s1[i], set);
		i++;
	}
	return (i);
}

static int	find_end_of_trimmed(char const *s1, char const *set)
{
	int	setcheck;
	int	i;

	i = ft_strlen(s1) - 1;
	setcheck = 1;
	while (i >= 0)
	{
		if (isset(s1[i], set) - setcheck == -1)
			break ;
		setcheck = isset(s1[i], set);
		i--;
	}
	return (i);
}

static int	isset(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}
