/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:35:13 by woosekim          #+#    #+#             */
/*   Updated: 2023/05/31 17:07:28 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define READLINE_LIBRARY

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <signal.h>
# include <limits.h>
# include <termios.h>
# include "libft.h"
# include "readline.h"
# include "history.h"


typedef struct termios	t_termios;

typedef struct s_env_set
{
	int				number;
	struct s_env	*head;
	struct s_env	*tail;
}					t_env_set;

typedef struct s_env
{
	char			*name;
	char			*value;
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
	AERROR,		// ambiguous redirect happened
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

typedef struct s_quotes
{
	int detect;
	char type;
} t_quotes;

int	exit_status;

t_env	*new_env_node(char *name, char *value, t_env *prev);
void	split_name_value(char *envp, char **name, char **value);
char *env_find_name(char *envp);
char *env_find_value(char *envp);
t_env	*env_list_init(char **envp, t_env *env_head, t_env *temp, t_env *new);

void signal_setup(void);
void ctrl_c_handler(int signum);
void ctrl_d_handler(t_env *env_head, char *line);
void here_signal_setup(void);
void here_ctrl_c_handler(int signum);
int shell_op(char *line, t_env **env_head);

t_token *token_list_init(char *str, t_token *token_head, t_token *temp, t_token *new);
void merge_redir(t_token **token_head);
t_token_type convert_type(char *str);

t_cmd *parser(t_token *token_head, t_env *env_head);
t_cmd *new_cmd_node(void);

t_here *repeat_heredocs(t_cmd *cmd_head);
t_here *do_a_heredoc(char *limiter);
char *nexist_name(void);
void write_heredoc(int fd, char *limiter);
t_here *free_n_return(char *str, int exit_code);
t_here *clear_here_n_return(t_here *here_head);
void here_add_bottom(t_here **here_head, t_here *here_doc);
void update_redirs(t_token *redirs, t_here *here_doc);

int exec_cmds(t_cmd *cmd_head, t_env **env_head);
int open_pipe(t_cmd *cmd);
int only_builtin_child(t_cmd *cmd, t_env **env_head);
int child(t_cmd *cmd, t_env **env_head);
char **words_lst_to_arr(t_cmd *cmd);
char *find_cmd_path(char *cmd, char **envp);
char **find_path_env(char **envp);
char *check_cmd_path(char *cmd, char **dirs);
void free_double_ptr(char **ptr);
int parent(int pid, t_cmd *cmd_head);
void clear_this_line(t_cmd *cmd_head, t_here *here_head);
int perror_return(char *str, int exit_code);

char **env_conv_arr(t_env *env_head);
void list_free(t_env *head);
void arr2d_free(char **arr);

int is_builtin(char *cmd);
int run_builtin(t_cmd *cmd, t_env **env_head);
int run_only_builtin(t_cmd *cmd, t_env **env_head);
int ft_echo(char **argv);
int ft_cd(t_cmd *cmd, t_env *env_head);
int ft_pwd(void);
int ft_export(t_cmd *cmd, t_env *env_head);
int ft_unset(t_cmd *cmd, t_env **env_head);
int ft_env(t_env *env_head);
int ft_exit(t_cmd *cmd, int exit_code);
int is_valid_name(char *str);
int env_set_value(t_env *env_head, char *name, char *value);
char *env_get_value(t_env *env_head, char *name);
int env_remove(t_env **env_head, char *name);

int unquote(t_token *token_head);
char *unquote_a_str(char *str);
int find_n_of_quotes(char *str);
int check_quotes2(char s, t_quotes *q);

void test_print_env(t_env *env_head);
void test_print_tokens(t_token *token_head);
void test_print_cmds(t_cmd *cmd_head);

#endif
