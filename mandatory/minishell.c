/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:34:03 by joonhlee          #+#    #+#             */
/*   Updated: 2023/05/19 13:00:28 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_env				*env_head;
	struct sigaction	act;
	char				*line;

	if (argc != 1 || argv[1] != 0)
		return (1);
	ft_bzero(&act, sizeof(act));
	act.__sigaction_u.__sa_handler = &sig_handle;
	sigaction(SIGINT, &act, NULL);
	env_head = env_list_init(envp, NULL, NULL, NULL);
	while (1)
	{
		line = readline("minishell> ");
		if (line == 0)
		{
			list_free(env_head);
			return (1);
		}
		if (ft_strcmp(line, "exit") == 0)
		{
			list_free(env_head);
			return (0);
		}
		if (line && *line)
			add_history(line);
		shell_op(line, env_head);
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
