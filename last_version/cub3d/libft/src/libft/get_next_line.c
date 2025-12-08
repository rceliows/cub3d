/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:47:06 by rceliows          #+#    #+#             */
/*   Updated: 2025/04/30 15:47:27 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

static char	*read_and_join(int fd, char *buffer, char *read_buf, ssize_t *bytes)
{
	char	*temp;

	*bytes = read(fd, read_buf, BUFFER_SIZE);
	if (*bytes < 0)
		return (free(buffer), NULL);
	read_buf[*bytes] = '\0';
	if (!buffer)
	{
		buffer = ft_strdup("");
		if (!buffer)
			return (NULL);
	}
	temp = ft_strjoin(buffer, read_buf);
	free(buffer);
	return (temp);
}

static char	*read_file(int fd, char *buffer)
{
	char	*read_buf;
	char	*result;
	ssize_t	bytes;

	read_buf = malloc(BUFFER_SIZE + 1);
	if (!read_buf)
		return (NULL);
	result = buffer;
	while (!result || !ft_strchr(result, '\n'))
	{
		result = read_and_join(fd, result, read_buf, &bytes);
		if (!result || bytes == 0)
			break ;
	}
	free(read_buf);
	return (result);
}

static size_t	find_newline(char *buffer)
{
	size_t	i;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	return (i);
}

static char	*extract_and_update(char **buffer, size_t i)
{
	char	*line;
	char	*new_buffer;

	if (!*buffer || !(*buffer)[0])
		return (NULL);
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	ft_memcpy(line, *buffer, i);
	line[i] = '\0';
	if (!(*buffer)[i])
	{
		free(*buffer);
		*buffer = NULL;
		return (line);
	}
	new_buffer = ft_strdup(*buffer + i);
	free(*buffer);
	*buffer = new_buffer;
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	size_t		i;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		if (buffer)
		{
			free(buffer);
			buffer = NULL;
		}
		return (NULL);
	}
	buffer = read_file(fd, buffer);
	if (!buffer)
		return (NULL);
	i = find_newline(buffer);
	return (extract_and_update(&buffer, i));
}
