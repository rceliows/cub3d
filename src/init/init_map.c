#include "../inc/cub3d.h"

int	extract_images(t_map *map, t_window *w)
{
	map->texture[0] = mlx_xpm_file_to_image(w->mlx, map->n_xpm_path,
			&map->texture_width[0], &map->texture_height[0]);
	if (!map->texture[0])
		return (0);
	map->texture[1] = mlx_xpm_file_to_image(w->mlx, map->s_xpm_path,
			&map->texture_width[1], &map->texture_height[1]);
	if (!map->texture[1])
		return (0);
	map->texture[3] = mlx_xpm_file_to_image(w->mlx, map->e_xpm_path,
			&map->texture_width[3], &map->texture_height[3]);
	if (!map->texture[3])
		return (0);
	map->texture[2] = mlx_xpm_file_to_image(w->mlx, map->w_xpm_path,
			&map->texture_width[2], &map->texture_height[2]);
	if (!map->texture[2])
		return (0);
	return (1);
}

void	set_world_map(t_map *map, int height, int width)
{
	map->world_map = malloc(sizeof(int *) * height);
	if (!map->world_map)
		return ;
	map->map_height = height;
	map->map_width = width;
}

static int	allocate_map_rows(t_map *map, int height, int width)
{
	int	y;

	y = 0;
	while (y < height)
	{
		map->world_map[y] = malloc(sizeof(int) * width);
		if (!map->world_map[y])
		{
			while (--y >= 0)
				free(map->world_map[y]);
			free(map->world_map);
			free(map);
			return (0);
		}
		y++;
	}
	return (1);
}

t_map	*init_map(int height, int width)
{
	t_map	*map;

	map = malloc(sizeof(t_map));
	if (!map)
		return (NULL);
	ft_memset(map, 0, sizeof(t_map));
	set_world_map(map, height, width);
	if (!map->world_map)
		return (free(map), NULL);
	if (!allocate_map_rows(map, height, width))
		return (NULL);
	return (map);
}
