/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 13:14:23 by joonhlee          #+#    #+#             */
/*   Updated: 2023/03/19 12:39:50 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_word(char c_tocheck, char c);
static int	count_word(const char *s, char c);
static int	split_exec(const char *s, char **split_s, char c, int len);
static void	ft_clear_previous_words(char **split_s, int error_check);

char	**ft_split(char const *s, char c)
{
	size_t	len_s;
	int		num_word;
	char	**split_s;
	int		error_check;

	len_s = ft_strlen(s);
	num_word = count_word(s, c);
	split_s = (char **)malloc((num_word + 1) * sizeof(char *));
	if (split_s == 0)
		return ((char **)0);
	error_check = split_exec(s, split_s, c, len_s);
	if (error_check < 0)
	{
		ft_clear_previous_words(split_s, error_check);
		return ((char **)0);
	}
	return (split_s);
}

static int	is_word(char c_tocheck, char c)
{
	int	i;

	i = 0;
	if (c_tocheck == '\0')
		return (0);
	if (c_tocheck == c)
		return (0);
	return (1);
}

static int	count_word(const char *s, char c)
{
	int	i;
	int	wordcheck;
	int	num_word;

	i = 0;
	wordcheck = 0;
	num_word = 0;
	while (s[i])
	{
		if (is_word(s[i], c) - wordcheck == 1)
			num_word += 1;
		wordcheck = is_word(s[i], c);
		i++;
	}
	return (num_word);
}

static int	split_exec(const char *s, char **split_s, char c, int len)
{
	int	s_ind;
	int	wordcheck;
	int	word_ind;
	int	mark[2];

	s_ind = 0;
	wordcheck = 0;
	word_ind = 0;
	while (s_ind <= len)
	{
		if (is_word(s[s_ind], c) - wordcheck == 1)
			mark[0] = s_ind;
		if (is_word(s[s_ind], c) - wordcheck == -1)
		{
			mark[1] = s_ind;
			split_s[word_ind] = ft_substr(s, mark[0], mark[1] - mark[0]);
			if (split_s[word_ind] == 0)
				return (-word_ind - 1);
			word_ind += 1;
		}
		wordcheck = is_word(s[s_ind++], c);
	}
	split_s[word_ind] = 0;
	return (word_ind);
}

static void	ft_clear_previous_words(char **split_s, int error_check)
{
	int	i;
	int	allocation_failed_ind;

	i = 0;
	allocation_failed_ind = -error_check - 1;
	while (i < allocation_failed_ind)
		free(split_s[i++]);
	free(split_s);
}
