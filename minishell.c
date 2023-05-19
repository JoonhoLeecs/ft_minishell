/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:34:03 by joonhlee          #+#    #+#             */
/*   Updated: 2023/05/08 16:24:43 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	struct sigaction	act;
	char				*line;

	ft_bzero(&act, sizeof(act));
	act.__sigaction_u.__sa_handler = &sig_handle;
	sigaction(SIGINT, &act, NULL);
	while (1)
	{
		line = readline("minishell> ");
		if (ft_strcmp(line, "exit") == 0)
			return (0);
		if (line && *line)
			add_history(line);
		free(line);
	}
	return (0);
}

void	sig_handle(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
	else
		return ;
}
