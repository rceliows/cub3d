#include "../inc/cub3d.h"

static int	process_header_or_blank(char *l, t_pmctx *c)
{
	int	result;

	if (!c->in_map && is_blank(l))
	{
		free(l);
		return (1);
	}
	if (!c->in_map)
	{
		result = check_n_set_header(l, c->game, &c->hdr_count);
		if (result == 1)
		{
			free(l);
			return (1);
		}
		if (result == -1)
		{
			free(l);
			return (-1);
		}
	}
	return (0);
}

static int	process_map_start(char *l, t_pmctx *c)
{
	if (c->in_map || c->hdr_count != 6)
		return (0);
	if (!check_if_valid(l))
	{
		ft_putstr_fd("Error: invalid character in map\n", 2);
		return (free(l), -1);
	}
	c->in_map = 1;
	if (push_map_line(&c->map, &c->map_count, &c->map_cap, l))
	{
		ft_putstr_fd("Error: failed to allocate map row\n", 2);
		return (free(l), -1);
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
		ft_putstr_fd("Error: empty line in middle of map\n", 2);
		return (free(l), -1);
	}
	if (!check_if_valid(l))
	{
		ft_putstr_fd("Error: invalid character in map line\n", 2);
		return (free(l), -1);
	}
	if (push_map_line(&c->map, &c->map_count, &c->map_cap, l))
	{
		ft_putstr_fd("Error: failed to allocate map row\n", 2);
		return (free(l), -1);
	}
	free(l);
	return (1);
}

static int	process_invalid_before_map(char *l, t_pmctx *c)
{
	if (c->in_map)
		return (0);
	if (c->hdr_count < 6)
		ft_putstr_fd("Error: invalid or duplicate header line\n", 2);
	else
		ft_putstr_fd("Error: unexpected content before map\n", 2);
	free(l);
	return (-1);
}

static int	process_line(char *line, t_pmctx *c)
{
	int	result;

	rstrip_nl(line);
	result = process_header_or_blank(line, c);
	if (result == 0)
		result = process_map_start(line, c);
	if (result == 0)
		result = process_map_rows(line, c);
	if (result == 0)
		result = process_invalid_before_map(line, c);
	return (result);
}

static char	**validate_parsed_map(t_pmctx *c)
{
	if (c->hdr_count != 6)
	{
		ft_putstr_fd("Error: incomplete header ", 2);
		ft_putstr_fd("(need NO, SO, WE, EA, F, C)\n", 2);
		return (ft_free_split(c->map), NULL);
	}
	if (!c->map || !c->map[0])
		return (ft_free_split(c->map),
			ft_putstr_fd("Error: no map found\n", 2), NULL);
	return (c->map);
}

char	**parse_map(char *file, t_data *game)
{
	char	*line;
	t_pmctx	c;
	int		result;

	c = (t_pmctx){NULL, 0, 0, 0, 0, 0, 0, game};
	c.fd = open(file, O_RDONLY);
	if (c.fd < 0)
		return (perror("Error"), NULL);
	line = get_next_line(c.fd);
	while (line)
	{
		result = process_line(line, &c);
		if (result == -1)
		{
			close(c.fd);
			get_next_line(-1);
			ft_free_split(c.map);
			return (NULL);
		}
		line = get_next_line(c.fd);
	}
	close(c.fd);
	get_next_line(-1);
	return (validate_parsed_map(&c));
}
