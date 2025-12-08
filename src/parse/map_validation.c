#include "../inc/cub3d.h"

static void	flood_fill(char **map_copy, int *player, int *map_size,
				int *params);

static void	fill_directions(char **map_copy, int *player, int *map_size,
		int *params)
{
	int	next_pos[2];
	int	px;
	int	py;

	px = player[0];
	py = player[1];
	next_pos[0] = px + params[0];
	next_pos[1] = py;
	flood_fill(map_copy, next_pos, map_size, params);
	next_pos[0] = px - params[0];
	next_pos[1] = py;
	flood_fill(map_copy, next_pos, map_size, params);
	next_pos[0] = px;
	next_pos[1] = py + params[1];
	flood_fill(map_copy, next_pos, map_size, params);
	next_pos[0] = px;
	next_pos[1] = py - params[1];
	flood_fill(map_copy, next_pos, map_size, params);
}

static void	flood_fill(char **map_copy, int *player, int *map_size, int *params)
{
	int	px;
	int	py;

	px = player[0];
	py = player[1];
	if (px < 0 || py < 0 || px >= map_size[0] || py >= map_size[1])
	{
		params[2] = 0;
		return ;
	}
	if (map_copy[px][py] == '1' || map_copy[px][py] == 'V')
		return ;
	if (map_copy[px][py] == ' ')
	{
		params[2] = 0;
		return ;
	}
	if (map_copy[px][py] == '0' || map_copy[px][py] == 'N'
		|| map_copy[px][py] == 'S' || map_copy[px][py] == 'W'
		|| map_copy[px][py] == 'E' || map_copy[px][py] == 'D'
		|| map_copy[px][py] == 'P' || map_copy[px][py] == 'A')
	{
		map_copy[px][py] = 'V';
		fill_directions(map_copy, player, map_size, params);
	}
}

static void	free_map(char **map, int rows)
{
	int	i;

	i = 0;
	while (i < rows)
	{
		free(map[i]);
		i++;
	}
	free(map);
}

static int	is_valid_tile(char c)
{
	return (c == '0' || c == 'N' || c == 'S' || c == 'W'
		|| c == 'E' || c == 'D' || c == 'P' || c == 'A');
}

int	map_check(char **map, int *player, int *map_size, int *iteration)
{
	char	**map_copy;
	int		params[3];

	params[0] = iteration[0];
	params[1] = iteration[1];
	params[2] = 1;
	map_copy = copy_map(map, map_size);
	if (!map_copy)
		return (0);
	if (map_copy[player[0]][player[1]] == ' ')
	{
		free_map(map_copy, map_size[0]);
		return (0);
	}
	if (!is_valid_tile(map_copy[player[0]][player[1]]))
		return (free_map(map_copy, map_size[0]), 0);
	flood_fill(map_copy, player, map_size, params);
	free_map(map_copy, map_size[0]);
	return (params[2]);
}
