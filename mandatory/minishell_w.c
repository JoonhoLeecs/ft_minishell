/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_w.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:35:16 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/19 08:26:16 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_leak(void)
{
	system("leaks --list minishell");
}

int	main(int argc, char **argv, char **envp)
{
	char	*str;
	t_env	*env_head;

	if (argc != 1)
		return (1);
	(void)argv;
	env_head = env_list_init(envp, NULL, NULL, NULL);
	// TEST ARR TO LIST (ENV)
	// while (env_head != NULL)
	// {
	// 	printf("%s", env_head->name);
	// 	printf("=");
	// 	printf("%s\n", env_head->content);
	// 	env_head = env_head->next;
	// }

	while (1)
	{
		str = readline("$> ");
	}

	// TEST LIST TO ARR (ENV)
	// char	**env_arr;
	// env_arr = env_conv_arr(env_head);
	// int	i = 0;
	// while (env_arr[i] != 0)
	// {
	// 	printf("%s\n", env_arr[i]);
	// 	i++;
	// }
	// arr2d_free(env_arr);

	list_free(env_head);
	//atexit(check_leak);
	return (0);
}
