/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:35:13 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/19 17:36:30 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define READLINE_LIBRARY

# include <stdio.h>
# include "libft.h"
# include "readline.h"
# include "history.h"
// # include "../libft/libft.h"
// # include "../readline/include/readline/readline.h"
// # include "../readline/include/readline/history.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <signal.h>

typedef struct s_env_set
{
	int	number;
	struct s_env *head;
	struct s_env *tail;
}				t_env_set;

typedef struct s_env
{
	char			*name;
	char			*content;
	struct s_env	*next;
	struct s_env	*prev;
}					t_env;

typedef enum e_token_type
{
	WORD = 0,
	PIPE,
	REDIR,		// '<' '>' '<<' '>>'
				// convert from REDIR + WORD into one of followings
	INFILE, 	// content "infile" meaning "< infile"
	OUTFILE,	// content "outfile" meaning "> outfile"
	HEREDOC,	// content "limiter" meaning "<< limiter"
	APPEND,		// content "outfile2 meaning ">> outfile2"
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;  //0 : word, 1 : pipe|, 2 : redir < > << >>
	char			*str;  //split result
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_cmd
{
	t_token			*words;		// list of word-type tokens
	t_token			*words_tail;
	t_token			*redirs;	// list of redirs-type tokens
	t_token			*redirs_tail;
	int				prev_pfd[2]; // pipes to communicate with prev command
	int				next_pfd[2]; // pipes to communicate with next command
	char			*cmd_path;  // first argv as command name
	char			**argv;     // convert from token list words
	struct s_cmd	*prev;		// to check if it's the first commmand
	struct s_cmd	*next;		// to check if it's the first commmand
}					t_cmd;

typedef struct s_here
{
	char			*filename;
	struct s_here	*next;
}					t_here;

// typedef struct s_redir
// {
// 	t_redir_type	r_type;  // 0 : infile<, 1 : outfile>, 2 : heredoc<<, 3 : append>>
// 	char			*file_name;  //type next token
// }	t_redir;

// typedef struct s_ast
// {

// 	t_redir			*redir;
// 	t_cmd			*cmd;
// 	struct s_ast	*left;
// 	struct s_ast	*right;  //left == NULL && right == NULL (->end node)
// }	t_ast;

t_env	*new_env_node(char *name, char *content, t_env *prev);
void	split_name_content(char *envp, char **name, char **content);
t_env	*env_list_init(char **envp, t_env *env_head, t_env *temp, t_env *new);

void	sig_handle(int signum);

int		shell_op(char *line, t_env *env_head);
t_token	*token_list_init(char *str, t_token *token_head, t_token *temp, t_token *new);
t_cmd	*parser(t_token *token_head, t_env *env_head);
t_here	*repeat_heredocs(t_cmd *cmd_head);
t_here *do_a_heredoc(char *limiter);
char *nexist_name(void);
void write_heredoc(int fd, char *limiter_nl);
t_here *free_n_return(char *str);
t_here *clear_here_n_return(t_here *here_head);
void	here_add_bottom(t_here **here_head, t_here *here_doc);
void update_redirs(t_token *redirs, t_here *here_doc);

int		exec_cmds(t_cmd *cmd_head, t_env *env_head);
void 	clear_this_line(t_cmd *cmd_head, t_here *here_head);

char	**env_conv_arr(t_env *env_head);
void	list_free(t_env *head);
void	arr2d_free(char **arr);


#endif
