/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhlee <joonhlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 14:42:35 by joonhlee          #+#    #+#             */
/*   Updated: 2023/03/29 10:18:05 by joonhlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef struct s_remains
{
	int					fd;
	char				*fd_remains;
	struct s_remains	*next;
}						t_remains;

char		*get_next_line(int fd);
t_remains	*ft_find_create_node(t_remains **head, int fd);
char		*ft_strjoin_free_gnl(char *line, char *buffer, int rcheck);
int			ft_pack_rmn(char *buffer, int rcheck, t_remains *node, char **line);
char		*ft_check_remains(t_remains *fd_node, int *rcheck);
char		*ft_free_reset(int *rcheck, char *prev);
size_t		ft_find_nchr_to_cpy(char *buffer, size_t size);
size_t		ft_strlen_gnl(const char *s);
void		*ft_memmove(void *dst, const void *src, size_t len);
void		ft_clear_fd_node(t_remains **head, int fd);

#endif
