#include "../inc/cub3d.h"

void	clenup_game(t_data *game)
{
	int	i;

	i = 0;
	if (game->map)
	{
		while (game->map[i])
		{
			free(game->map[i]);
			i++;
		}
		free(game->map);
	}
	if (game->north_texture)
		free(game->north_texture);
	if (game->south_texture)
		free(game->south_texture);
	if (game->west_texture)
		free(game->west_texture);
	if (game->east_texture)
		free(game->east_texture);
	free(game);
}

void	free_world_map(t_map *map)
{
	int	y;

	if (!map->world_map)
		return ;
	y = 0;
	while (y < map->map_height)
	{
		if (map->world_map[y])
			free(map->world_map[y]);
		y++;
	}
	free(map->world_map);
}

void	error_exit(t_cub3d *cub3d)
{
	printf("Error: Initialization failed\n");
	cleanup_cub3d(cub3d);
	exit(1);
}
