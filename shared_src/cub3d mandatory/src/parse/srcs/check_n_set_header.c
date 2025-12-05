/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_n_set_header.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 19:43:07 by rceliows          #+#    #+#             */
/*   Updated: 2025/12/05 19:43:12 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char	*dup_trim_right(const char *p)
{
	int		len;
	char	*s;
	int		start;
	int		end;

	if (!p)
		return (NULL);
	len = ft_strlen(p);
	start = 0;
	while (start < len && (p[start] == ' ' || p[start] == '\t'))
		start++;
	end = len;
	while (end > start && (p[end - 1] == ' ' || p[end - 1] == '\t' || p[end
				- 1] == '\n' || p[end - 1] == '\r'))
		end--;
	s = ft_substr(p, start, end - start);
	return (s);
}

static char	*skip_ws(char *s)
{
	while (*s == ' ' || *s == '\t')
		s++;
	return (s);
}

static int	set_texture(char *path, char **texture, int *hdr_count)
{
	if (*texture)
		return (0);
	*texture = dup_trim_right(path);
	if (!*texture)
		return (0);
	(*hdr_count)++;
	return (1);
}

static int	set_color(char *path, int rgb[3], int *hdr_count, int *flag)
{
	if (*flag)
		return (0);
	if (!parse_rgb(path, rgb))
		return (0);
	(*flag) = 1;
	(*hdr_count)++;
	return (1);
}

int	check_n_set_header(char *line, t_data *g, int *hdr_count)
{
	char	*p;

	p = skip_ws(line);
	if (!ft_strncmp(p, "NO ", 3) && set_texture(p + 3, &g->north_texture,
			hdr_count))
		return (1);
	if (!ft_strncmp(p, "SO ", 3) && set_texture(p + 3, &g->south_texture,
			hdr_count))
		return (1);
	if (!ft_strncmp(p, "EA ", 3) && set_texture(p + 3, &g->east_texture,
			hdr_count))
		return (1);
	if (!ft_strncmp(p, "WE ", 3) && set_texture(p + 3, &g->west_texture,
			hdr_count))
		return (1);
	if (!ft_strncmp(p, "F ", 2) && set_color(p + 2, g->floor, hdr_count,
			&g->have_floor))
		return (1);
	if (!ft_strncmp(p, "C ", 2) && set_color(p + 2, g->ceiling, hdr_count,
			&g->have_ceiling))
		return (1);
	return (0);
}
