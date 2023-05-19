/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 13:32:16 by joonhlee          #+#    #+#             */
/*   Updated: 2023/04/28 15:01:52 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*check_heredoc(char **argv, int fci)
{
	int		fd;
	char	*here_doc;
	char	*limiter;

	if (fci == 2)
		return (argv[1]);
	here_doc = nexist_name();
	if (here_doc == 0)
		return (0);
	fd = open(here_doc, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd == -1)
		return (free_n_return(here_doc));
	limiter = ft_strjoin(argv[2], "\n");
	if (limiter == 0)
		return (free_n_return(here_doc));
	write_heredoc(fd, limiter);
	free(limiter);
	close(fd);
	return (here_doc);
}

char	*nexist_name(void)
{
	int		check;
	char	*ind;
	char	*here_doc;

	check = 0;
	while (check >= 0)
	{
		ind = ft_itoa(check++);
		if (ind == 0)
			return (0);
		here_doc = ft_strjoin("here_doc", ind);
		free(ind);
		if (here_doc == 0)
			return (0);
		if (access(here_doc, F_OK) != 0)
			return (here_doc);
		free(here_doc);
	}
	return (0);
}

void	write_heredoc(int fd, char *limiter)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("pipe heredoc> ", STDOUT_FILENO);
		line = get_next_line(0);
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		free(line);
	}
}

char	*free_n_return(char *str)
{
	free(str);
	return (0);
}
