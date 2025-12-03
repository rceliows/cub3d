#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

static void	flood_fill(char **map_copy, int *player, int *map_size,
					int *params)
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
	if (map_copy[px][py] == '0' || map_copy[px][py] == 'N')
	{
		map_copy[px][py] = 'V';
		fill_directions(map_copy, player, map_size, params);
	}
}

static char	**copy_map(char **map, int *map_size)
{
	char	**map_copy;
	int		i;
	int		j;

	map_copy = malloc(sizeof(char *) * map_size[0]);
	if (!map_copy)
		return (NULL);
	i = 0;
	while (i < map_size[0])
	{
		map_copy[i] = malloc(sizeof(char) * map_size[1]);
		if (!map_copy[i])
		{
			while (--i >= 0)
				free(map_copy[i]);
			free(map_copy);
			return (NULL);
		}
		j = 0;
		while (j < map_size[1])
		{
			map_copy[i][j] = map[i][j];
			j++;
		}
		i++;
	}
	return (map_copy);
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

int	find_char_position(char **map, int *map_size, char c, int *position)
{
	int	i;
	int	j;

	i = 0;
	while (i < map_size[0])
	{
		j = 0;
		while (j < map_size[1])
		{
			if (map[i][j] == c)
			{
				position[0] = i;
				position[1] = j;
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

/*
params[0] = iteration_x
params[1] = iteration_y
params[2] = is_closed
*/
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
	if (map_copy[player[0]][player[1]] != '0'
		&& map_copy[player[0]][player[1]] != 'N')
	{
		free_map(map_copy, map_size[0]);
		return (0);
	}
	flood_fill(map_copy, player, map_size, params);
	free_map(map_copy, map_size[0]);
	return (params[2]);
}

int	main(void)
{
	int	valid;
	int	player[2];
	int	map_size[2] = {14, 33};
	int	iteration[2] = {1, 1};
	char map_data[14][33] = {
		{'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
		{'1','1','1','1','1','1','1','1','1','0','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','1','1','1'},
		{'1','1','1','1','1','1','1','1','1','0','1','1','0','0','0','0','0','1','1','1','0','0','0','0','0','0','0','0','0','0','1','1','1'},
		{'1','1','1','1','1','1','1','1','1','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1','1','1'},
		{'1','1','1','1','1','1','1','1','1','0','1','1','0','0','0','0','0','1','1','1','0','0','0','0','0','0','0','0','0','0','1','1','1'},
		{'1','0','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','1','1','1','0','1','1','1','1','1','1','1','1','1','1','1','1'},
		{'1','1','1','1','1','1','1','1','0','1','1','1','1','1','1','1','1','1','0','1','1','1','0','0','0','0','0','0','1','0','0','0','1'},
		{'1','1','1','1','1','1','1','1','0','1','1','1','1','1','1','1','1','1','0','1','1','1','0','1','0','1','0','0','1','0','0','0','1'},
		{'1','1','1','1','1','1','0','0','0','0','0','0','1','1','0','1','0','1','0','1','1','1','0','0','0','0','0','0','1','0','0','N','1'},
		{'1','1','1','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','1','0','0','0','1'},
		{'1','1','1','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1','1','0','1','0','1','0','0','0','0','0','0','0','1'},
		{'1','1','1','1','0','0','0','0','0','1','1','1','0','1','0','1','0','1','1','1','1','1','0','1','1','1','1','0','0','0','1','1','1'},
		{'1','1','1','1','1','1','1','1','0','1','1','1','1','1','1','1','0','1','0','1','1','1','0','1','1','1','1','0','0','0','0','0','1'},
		{'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
	};
	
	char **map = malloc(sizeof(char *) * 14);
	for (int i = 0; i < 14; i++)
	{
		map[i] = malloc(sizeof(char) * 33);
		for (int j = 0; j < 33; j++)
			map[i][j] = map_data[i][j];
	}
	if (!find_char_position(map, map_size, 'N', player))
	{
		printf("Character 'N' not found in map\n");
		free_map(map, 14);
		return (0);
	}
	printf("Player position: [%d, %d]\n", player[0], player[1]);
	valid = map_check(map, player, map_size, iteration);
	printf("Map is closed: %i\n", valid);
	free_map(map, 14);
	return (valid);
}
