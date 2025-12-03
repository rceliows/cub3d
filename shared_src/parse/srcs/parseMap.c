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
// #include "get_next_line.h"
// #include "libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	has_cub_extension(const char *map)
{
	size_t	len;

	if (!map)
		return (0);
	len = ft_strlen(map);
	if (len < 5)
		return (0);
	return (ft_strncmp(map + len - 4, ".cub", 4) == 0);
}

void	parse_rgb(char *line, int floor[3])
{
	char	**n_split;
	int		i;

	n_split = NULL;
	n_split = ft_split(line, ',');
	if (!n_split)
		return ;
	i = 0;
	while (n_split[i])
	{
		floor[i] = ft_atoi(n_split[i]);
		i++;
	}
}
int	is_blank(const char *s)
{
	int	i;

	i = 0;
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r')
		i++;
	return (s[i] == '\0');
}

static char	*dup_trim_right(const char *p)
{
	int		len;
	char	*s;

	s = ft_strdup(p);
	if (!s)
		return (NULL);
	len = (int)ft_strlen(s);
	while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r' || s[len
			- 1] == ' ' || s[len - 1] == '\t'))
		s[--len] = '\0';
	return (s);
}

static int	check_n_set_header(char *line, t_data *g, int *hdr_count)
{
	if (!ft_strncmp(line, "NO ", 3) && !g->North_texture)
	{
		g->North_texture = dup_trim_right(line + 3);
		(*hdr_count)++;
		return (1);
	}
	if (!ft_strncmp(line, "SO ", 3) && !g->South_texture)
	{
		g->South_texture = dup_trim_right(line + 3);
		(*hdr_count)++;
		return (1);
	}
	if (!ft_strncmp(line, "EA ", 3) && !g->East_texture)
	{
		g->East_texture = dup_trim_right(line + 3);
		(*hdr_count)++;
		return (1);
	}
	if (!ft_strncmp(line, "WE ", 3) && !g->West_texture)
	{
		g->West_texture = dup_trim_right(line + 3);
		(*hdr_count)++;
		return (1);
	}
	if (!ft_strncmp(line, "F ", 2) && !g->have_floor)
	{
		parse_rgb(line + 2, g->floor);
		g->have_floor = 1;
		(*hdr_count)++;
		return (1);
	}
	if (!ft_strncmp(line, "C ", 2) && !g->have_ceiling)
	{
		parse_rgb(line + 2, g->ceiling);
		g->have_ceiling = 1;
		(*hdr_count)++;
		return (1);
	}
	return (0);
}

int	check_if_valid(const char *s)
{
	int	i;
	int	seen;

	i = 0;
	seen = 0;
	while (s[i])
	{
		if (s[i] != '0' && s[i] != '1' && s[i] != 'N' && s[i] != 'S'
			&& s[i] != 'E' && s[i] != 'W' && s[i] != ' ' && s[i] != '\n'
			&& s[i] != '\t')
			return (0);
		if (s[i] != ' ' && s[i] != '\n' && s[i] != '\t')
			seen = 1;
		i++;
	}
	return (seen);
}

int	ensure_cap(char ***map, int *cap, int need)
{
	char	**n;
	int		i;

	if (need + 1 <= *cap)
		return (0);
	if (*cap == 0)
		*cap = 8;
	else
		*cap = *cap * 2;
	while (*cap < need + 1)
		*cap = *cap * 2;
	n = (char **)malloc(sizeof(char *) * (*cap + 1));
	if (!n)
		return (1);
	i = 0;
	if (*map)
	{
		while ((*map)[i])
		{
			n[i] = (*map)[i];
			i++;
		}
		free(*map);
	}
	n[i] = NULL;
	*map = n;
	return (0);
}

int	push_map_line(char ***map, int *map_count, int *map_cap, char *line)
{
	rstrip_nl(line);
	if (is_blank(line))
		return (1);
	if (ensure_cap(map, map_cap, *map_count + 1))
		return (1);
	(*map)[*map_count] = ft_strdup(line);
	if (!(*map)[*map_count])
		return (1);
	(*map_count)++;
	(*map)[*map_count] = NULL;
	return (0);
}

char	**parse_map(char *file, t_data *game)
{
	int		fd;
	char	*line;
	char	**map;
	int		map_count;
	int		in_map;
	int		hdr_count;
	int		map_cap;

	map = NULL;
	map_cap = 0;
	hdr_count = 0;
	map_count = 0;
	in_map = 0;
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror("Error: could not open map file");
		exit(EXIT_FAILURE);
	}
	while ((line = get_next_line(fd)))
	{
		if (!in_map)
		{
			if (is_blank(line))
			{
				free(line);
				continue ;
			}
			if (check_n_set_header(line, game, &hdr_count))
			{
				free(line);
				continue ;
			}
			if (hdr_count == 6 && check_if_valid(line))
				in_map = 1;
			else
			{
				free(line);
				ft_putstr_fd("Error: invalid line before map\n", 2);
				close(fd);
				exit(EXIT_FAILURE);
			}
		}
		if (!check_if_valid(line) || push_map_line(&map, &map_count, &map_cap,
				line))
		{
			free(line);
			ft_free_split(map);
			ft_putstr_fd("Error: map not valid", 2);
			close(fd);
			exit(EXIT_FAILURE);
		}
		free(line);
	}
	close(fd);
	if (hdr_count != 6 || !map || !map[0])
	{
		ft_free_split(map);
		ft_putstr_fd("Error: missing header or empty map", 2);
		exit(EXIT_FAILURE);
	}
	return (map);
}

int	validate_map(t_data *game)
{
	int		map_size[2];
	int		player[2];
	int		iteratio[2];
	int		row;
	int		col;
	char	dir;
	int max_col;

	max_col = 0;
	row = 0;
	while (game->map[row])
	{
		int len = (int)ft_strlen(game->map[row]);
		if (len > max_col)
			max_col = len;
		row++;
	}
	map_size[0] = row;
	col = (int)ft_strlen(game->map[0]);
	map_size[1] = max_col;
	iteratio[0] = 1;
	iteratio[1] = 1;
	dir = 0;
	if (find_char_position(game->map, map_size, 'N', player))
		dir = 'N';
	else if (find_char_position(game->map, map_size, 'S', player))
		dir = 'S';
	else if (find_char_position(game->map, map_size, 'E', player))
		dir = 'E';
	else if (find_char_position(game->map, map_size, 'W', player))
		dir = 'W';
	else
	{
		ft_putstr_fd("Error: no player found\n", 2);
		return (0);
	}
	if (!map_check(game->map, player, map_size, iteratio))
	{
		ft_putstr_fd("Error: fails map check\n", 2);
		return (0);
	}
	game->player_y = player[0];
	game->player_x = player[1];
	game->player_dir = dir;
	return (1);
}

int	has_xpm_extension(const char *path)
{
	size_t	len;

	if (!path)
		return (0);
	len = ft_strlen(path);
	if (len < 5)
		return (0);
	return (ft_strncmp(path + len - 4, ".xpm", 4) == 0);
}
int	is_valid_texture_path(const char *raw)
{
	int		fd;
	char	*path;

	if (!raw)
		return (0);
	path = (char *)raw;
	while (*path == ' ' || *path == '\t')
		path++;
	if (!has_xpm_extension(path))
		return (0);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (0);
	close(fd);
	return (1);
}
int	validate_textures(t_data *game)
{
	if (!game->North_texture || !game->South_texture || !game->East_texture
		|| !game->West_texture)
	{
		ft_putstr_fd("Error: missing texture paths\n", 2);
		return (0);
	}
	if (!is_valid_texture_path(game->North_texture))
	{
		ft_putstr_fd("Error: invalid NO texture\n", 2);
		return (0);
	}
	if (!is_valid_texture_path(game->South_texture))
	{
		ft_putstr_fd("Error: invalid SO texture\n", 2);
		return (0);
	}
	if (!is_valid_texture_path(game->West_texture))
	{
		ft_putstr_fd("Error: invalid WE texture\n", 2);
		return (0);
	}
	if (!is_valid_texture_path(game->East_texture))
	{
		ft_putstr_fd("Error: invalid EA texture\n", 2);
		return (0);
	}
	return (1);
}

void	debug_print_game_info(t_data *g)
{
	printf("----- Parsed Map Info -----\n");
	printf("North texture:  %s\n",
		g->North_texture ? g->North_texture : "(null)");
	printf("South texture:  %s\n",
		g->South_texture ? g->South_texture : "(null)");
	printf("West texture:   %s\n",
		g->West_texture ? g->West_texture : "(null)");
	printf("East texture:   %s\n",
		g->East_texture ? g->East_texture : "(null)");
	printf("Floor color:    R:%d G:%d B:%d\n", g->floor[0], g->floor[1],
		g->floor[2]);
	printf("Ceiling color:  R:%d G:%d B:%d\n", g->ceiling[0], g->ceiling[1],
		g->ceiling[2]);
	printf("Player pos:     x=%d, y=%d, dir=%c\n", g->player_x, g->player_y,
		g->player_dir);
	printf("Map preview:\n");
	for (int i = 0; g->map && g->map[i]; i++)
		printf("%s", g->map[i]);
	printf("\n");
	printf("----------------------------\n");
}

// int	main(int ac, char **av)
// {
// 	t_data	game;

// 	ft_memset(&game, 0, sizeof(t_data));
// 	if (ac != 2)
// 		return (ft_putstr_fd("Proper usage: ./cube3d map.cub", 2), 1);
// 	if (!has_cub_extension(av[1]))
// 		return (ft_putstr_fd("Proper usage: ./cube3d map.cub", 2), 1);
// 	game.map = parse_map(av[1], &game);
// 	if (!game.map[0] || !game.map)
// 	{
// 		ft_putstr_fd("Error: empty map\n", 2);
// 		exit(EXIT_FAILURE);
// 	}
// 	if (!validate_textures(&game))
// 	{
// 		ft_putstr_fd("Error: non textures\n", 2);
// 		exit(EXIT_FAILURE);
// 	}
// 	if (!validate_map(&game))
// 	{
// 		ft_putstr_fd("Error: non valid map\n", 2);
// 		exit(EXIT_FAILURE);
// 	}
// 	debug_print_game_info(&game);
// 	return (0);
// }

int	verify_map(const char *path, t_data *game)
{
	ft_memset(game, 0, sizeof(t_data));
	if (!has_cub_extension(path))
	{
		ft_putstr_fd("Proper usage: ./cube3d map.cub\n", 2);
		return (0);
	}
	game->map = parse_map((char *)path, game);
	if (!game->map || !game->map[0])
	{
		ft_putstr_fd("Error: empty map\n", 2);
		return (0);
	}
	if (!validate_textures(game))
	{
		ft_putstr_fd("Error: non textures\n", 2);
		return (0);
	}
	if (!validate_map(game))
	{
		ft_putstr_fd("Error: non valid map\n", 2);
		return (0);
	}
	return (1);
}
