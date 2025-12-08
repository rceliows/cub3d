#include "../inc/cub3d.h"

static void	free_partial_copy(char **map_copy, int rows)
{
	while (--rows >= 0)
		free(map_copy[rows]);
	free(map_copy);
}

static int	alloc_row_copy(char **map_copy, char **map, int row, int *map_size)
{
	int	j;
	int	len;

	map_copy[row] = malloc(sizeof(char) * map_size[1]);
	if (!map_copy[row])
		return (0);
	len = ft_strlen(map[row]);
	j = 0;
	while (j < map_size[1])
	{
		if (j < len)
			map_copy[row][j] = map[row][j];
		else
			map_copy[row][j] = ' ';
		j++;
	}
	return (1);
}

static char	**alloc_map_copy(int height)
{
	char	**map_copy;

	map_copy = malloc(sizeof(char *) * height);
	if (!map_copy)
		return (NULL);
	return (map_copy);
}

char	**copy_map(char **map, int *map_size)
{
	char	**map_copy;
	int		row;

	map_copy = alloc_map_copy(map_size[0]);
	if (!map_copy)
		return (NULL);
	row = 0;
	while (row < map_size[0])
	{
		if (!alloc_row_copy(map_copy, map, row, map_size))
		{
			free_partial_copy(map_copy, row);
			return (NULL);
		}
		row++;
	}
	return (map_copy);
}
