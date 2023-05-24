/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:34:03 by joonhlee          #+#    #+#             */
/*   Updated: 2023/05/24 10:57:32 by joonhlee         ###   ########.fr       */
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
	//test_print_env(env_head);
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
		shell_op(line, &env_head);
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

void	test_print_env(t_env *env_head)
{
	t_env	*env_iter;

	env_iter = env_head;
	while (env_iter)
	{
		printf("env|key=%s|value=%s\n", env_iter->name, env_iter->value);
		env_iter = env_iter->next;
	}
}

void	test_print_tokens(t_token *token_head)
{
	t_token	*token_iter;

	token_iter = token_head;
	while (token_iter)
	{
		printf("token_type=%d|token_value=%s|prev=%p|current=%p|next=%p\n",
			token_iter->type, token_iter->str,
			token_iter->prev, token_iter, token_iter->next);
		token_iter = token_iter->next;
	}
}

void test_print_cmds(t_cmd *cmd_head)
{
	t_cmd *cmd_iter;
	t_token	*token_iter;

	cmd_iter = cmd_head;
	while (cmd_iter)
	{
		token_iter = cmd_iter->words;
		while (token_iter)
		{
			printf("token_type=%d|token_value=%s|prev=%p|current=%p|next=%p\n",
				token_iter->type, token_iter->str,
				token_iter->prev, token_iter, token_iter->next);
			token_iter = token_iter->next;
		}
		token_iter = cmd_iter->redirs;
		while (token_iter)
		{
			printf("token_type=%d|token_value=%s|prev=%p|current=%p|next=%p\n",
				token_iter->type, token_iter->str,
				token_iter->prev, token_iter, token_iter->next);
			token_iter = token_iter->next;
		}
		cmd_iter = cmd_iter->next;
	}
}
