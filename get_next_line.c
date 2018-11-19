/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohamed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 06:37:16 by amohamed          #+#    #+#             */
/*   Updated: 2018/11/12 23:27:01 by amohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	ft_remove_first_line(char **begin, int amt_read)
{
	char	*temp;
	int		new_size;

	temp = *begin;
	if (amt_read < BUFF_SIZE && !ft_strchr(*begin, '\n'))
		*begin = ft_memalloc(1);
	else
	{
		new_size = ft_strlen(*begin) - (ft_strchr(*begin, '\n') - *begin);
		*begin = ft_memalloc(new_size);
		*begin = ft_strcpy(*begin, ft_strchr(temp, '\n') + 1);
	}
	free(temp);
}

void	ft_reallocate_for_read(char **begin, char **rest)
{
	char	*orig;

	orig = *begin;
	*begin = ft_memalloc(ft_strlen(orig) + BUFF_SIZE + 1);
	*begin = ft_strcpy(*begin, orig);
	*rest = *begin + ft_strlen(orig);
	free(orig);
}

void	ft_split_line_from_begin(char **line, char **begin, int amt_read)
{
	int	new_size;

	if (ft_strchr(*begin, '\n'))
	{
		new_size = ft_strchr(*begin, '\n') - *begin + 1;
		*line = (char*)ft_memalloc(sizeof(char) * new_size);
		*line = ft_strncpy(*line, *begin, new_size - 1);
		ft_remove_first_line(begin, amt_read);
	}
	else if (!ft_strchr(*begin, '\n'))
	{
		*line = *begin;
		*begin = NULL;
	}
}

char	**ft_find_read_for_given_fd(t_list **begin_list, int fd)
{
	t_list	*ptr;
	t_list	*new;

	ptr = *begin_list;
	while (ptr)
	{
		if ((int)(ptr->content_size) == fd)
			return ((char**)&(ptr->content));
		ptr = ptr->next;
	}
	new = ft_lstnew(NULL, 0);
	new->content_size = fd;
	ft_lstadd(begin_list, new);
	return ((char**)(&(new->content)));
}

int		get_next_line(const int fd, char **line)
{
	char			*rest;
	int				amt_read;
	static t_list	*begin_list;
	char			**begin;

	begin = ft_find_read_for_given_fd(&begin_list, fd);
	amt_read = BUFF_SIZE;
	if (!*begin && !(*begin = ft_memalloc(1)))
		return (-1);
	rest = *begin;
	while (!ft_strchr(rest, '\n') && amt_read == BUFF_SIZE)
	{
		ft_reallocate_for_read(begin, &rest);
		amt_read = read(fd, rest, BUFF_SIZE);
	}
	if (amt_read == -1)
		return (-1);
	if (begin[0][0] == '\0' && amt_read == 0)
		return (0);
	ft_split_line_from_begin(line, begin, amt_read);
	return (1);
}
