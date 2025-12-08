#include "../inc/cub3d.h"

t_door_manager	*get_door_manager(void)
{
	static t_door_manager	manager = {NULL, 0, 0};

	return (&manager);
}

int	is_door(t_map *map, int x, int y)
{
	if (x < 0 || x >= map->map_height || y < 0 || y >= map->map_width)
		return (0);
	return (map->world_map[x][y] == 2);
}
