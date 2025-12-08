/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseMap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmarchan <nmarchan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 12:13:42 by nmarchan          #+#    #+#             */
/*   Updated: 2025/10/27 12:27:15 by nmarchan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	process_header_or_blank(char *l, t_pmctx *c)
{
	if (!c->in_map && is_blank(l))
	{
		free(l);
		return (1);
	}
	if (!c->in_map && check_n_set_header(l, c->game, &c->hdr_count))
	{
		free(l);
		return (1);
	}
	return (0);
}

static int	process_map_start(char *l, t_pmctx *c)
{
	if (c->in_map || c->hdr_count != 6)
		return (0);
	if (!check_if_valid(l))
	{
		ft_putstr_fd("Error: invalid map start\n", 2);
		return (free(l), 1);
	}
	c->in_map = 1;
	if (push_map_line(&c->map, &c->map_count, &c->map_cap, l))
	{
		ft_putstr_fd("Error: invalid map row\n", 2);
		return (free(l), 1);
	}
	free(l);
	return (1);
}

static int	process_map_rows(char *l, t_pmctx *c)
{
	if (!c->in_map)
		return (0);
	if (is_blank(l))
	{
		c->gap = 1;
		return (free(l), 1);
	}
	if (c->gap)
	{
		ft_putstr_fd("Error: blank line inside map\n", 2);
		return (free(l), 1);
	}
	if (!check_if_valid(l) || push_map_line(&c->map, &c->map_count, &c->map_cap,
			l))
	{
		ft_putstr_fd("Error: invalid map line\n", 2);
		return (free(l), 1);
	}
	free(l);
	return (1);
}

static int	process_invalid_before_map(char *l, t_pmctx *c)
{
	if (c->in_map)
		return (0);
	ft_putstr_fd("Error: invalid line before map\n", 2);
	free(l);
	return (1);
}

char	**parse_map(char *file, t_data *game)
{
	char	*line;
	t_pmctx	c;

	c = (t_pmctx){NULL, 0, 0, 0, 0, 0, 0, game};
	c.fd = open(file, O_RDONLY);
	c.game = game;
	if (c.fd < 0)
		return (perror("Error: could not open map file"), NULL);
	line = get_next_line(c.fd);
	while (line)
	{
		rstrip_nl(line);
		if (process_header_or_blank(line, &c) || process_map_start(line, &c)
			|| process_map_rows(line, &c))
			line = get_next_line(c.fd);
		else if (process_invalid_before_map(line, &c))
			return (close(c.fd), get_next_line(-1), ft_free_split(c.map), NULL);
		else
			line = get_next_line(c.fd);
	}
	close(c.fd);
	if (c.hdr_count != 6 || !c.map || !c.map[0])
		return (ft_free_split(c.map), get_next_line(-1),
			ft_putstr_fd("Error: missing header or empty map\n", 2), NULL);
	return (get_next_line(-1), c.map);
}
