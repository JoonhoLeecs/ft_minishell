CC = cc
CFLAGS = -Wall -Wextra -Werror
# SRC_DIR = src/
SRC = minishell.c
# SRC_BONUS = pipex_bonus.c \
# 		pipex2_bonus.c \
# 		pipex3_bonus.c \
# 		heredoc_bonus.c \
# 		getnextline/get_next_line.c \
# 		getnextline/get_next_line_utils.c
OBJECTS = $(SRC:.c=.o)
# OBJECTS_BONUS = $(SRC_BONUS:.c=.o)
HEADER = pipex_bonus.h libft/libft.h getnextline/get_next_line.h
# HEADER_BONUS = pipex_bonus.h libft/libft.h getnextline/get_next_line.h
NAME = minishell
LIBFT = ./libft/libft.a
DIR_LIBFT = ./libft/
# LDFLAGS = -L./libft -lft -L./readline -lhistory -lreadline
# INCLUDES = -I./libft -I./getnextline -I./readline
# LDFLAGS = -L/Users/$(USER)/.brew/opt/readline/lib -lhistory -lreadline -L./libft -lft
LDFLAGS = -L/Users/$(USER)/.brew/opt/readline/lib -lreadline -L./libft -lft
INCLUDES = -I/Users/$(USER)/.brew/opt/readline/include -I./libft -I./getnextline
# CPPFLAGS = -I /Users/$(USER)/.brew/opt/readline/include

.PHONY: all bonus clean fclean re

all : MAKE_ALL

%.o: %.c
	$(CC) $(CFLAGS) -c $< $(INCLUDES) -o $@

$(NAME) : MAKE_ALL

MAKE_ALL : $(LIBFT) $(OBJECTS) $(HEADER)
			$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) $(LDFLAGS) $(INCLUDES)
			@touch MAKE_ALL
			@rm -f MAKE_BONUS

$(LIBFT) :
	$(MAKE) bonus -C $(DIR_LIBFT)

bonus : MAKE_BONUS

MAKE_BONUS : $(LIBFT) $(OBJECTS_BONUS) $(HEADER_BONUS)
			cc $(CFLAGS) $(OBJECTS_BONUS) -o $(NAME) $(LDFLAGS) $(INCLUDES)
			@touch MAKE_BONUS
			@rm -f MAKE_ALL

clean :
	$(MAKE) clean -C $(DIR_LIBFT)
	rm -f $(OBJECTS)
	rm -f $(OBJECTS_BONUS)
	rm -f $(LIBFT)
	@rm -f MAKE_ALL MAKE_BONUS

fclean :
	$(MAKE) fclean -C $(DIR_LIBFT)
	$(MAKE) clean
	rm -f $(NAME)

re :
	$(MAKE) fclean
	$(MAKE) all
