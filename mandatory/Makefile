NAME = minishell
#NAME = ../minishell

CC = cc
CFLAGS = -Werror -Wall -Wextra -g
LDFLAGS = 	-L../libft -lft \
			-L../readline/lib -lreadline \
			-L../readline/lib -lhistory

MAKE = make
MFLAGS = -C

RM = rm
RMFLAGS = -rf

LIBFT_DIR = ../libft/
LIBFT_LIB = $(LIBFT_DIR)libft.a

READLINE_DIR = ../readline/
READLINE_LIB = $(READLINE_DIR)lib/libreadline.a $(READLINE_DIR)lib/libhistory.a

HEADER = \
minishell.h \
$(LIBFT_DIR)libft.h \
../readline/include/readline/readline.h \
../readline/include/readline/history.h

INCLUDES = -I../readline/include/readline -I../libft
# -I../getnextline

MANDATORY_SRCS = \
minishell.c \
minishell_utils.c \
env_init.c \
env_conv_arr.c \
token_init.c \
shell_op.c \
parser.c \
heredoc.c \
find_cmd_path.c \
builtin.c \
unquote.c
# ../getnextline/get_next_line.c \
# ../getnextline/get_next_line_utils.c

MANDATORY_OBJS = $(MANDATORY_SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(LIBFT_LIB) $(READLINE_LIB) $(MANDATORY_OBJS) $(HEADER)
	$(CC) $(CFLAGS) -o $@ $(MANDATORY_OBJS) $(LDFLAGS)

$(LIBFT_LIB) :
	$(MAKE) $(MFLAGS) $(LIBFT_DIR) bonus

$(READLINE_LIB) :
	$(MAKE) $(MFLAGS) $(READLINE_DIR) all

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean :
	$(MAKE) $(MFLAGS) $(LIBFT_DIR) clean
	$(MAKE) $(MFLAGS) $(READLINE_DIR) clean
	$(RM) $(RMFLAGS) $(MANDATORY_OBJS)

fclean : clean
	$(MAKE) $(MFLAGS) $(LIBFT_DIR) fclean
	$(MAKE) $(MFLAGS) $(READLINE_DIR) fclean
	$(RM) $(RMFLAGS) $(NAME)

re : fclean all

.PHONY : all clean fclean re
