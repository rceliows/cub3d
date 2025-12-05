/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 16:38:29 by rceliows          #+#    #+#             */
/*   Updated: 2025/10/29 16:38:30 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

// t_map	*init_map(t_window *w)
// {
// 	t_map *map;
// 	int		y;
// 	int		x;
// 	(void)w;
// 	map = malloc(sizeof(t_map));
// 	if (!map)
// 		return (NULL);
// 	ft_memset(map, 0, sizeof(t_map));
// 	y = 0;
// 	while (y < MAPHEIGHT)
// 	{
// 		x = 0;
// 		while (x < MAPWIDTH)
// 		{
// 			map->world_map[y][x] = 1;
// 			x++;
// 		}
// 		y++;
// 	}
// 	return (map);
// }

void	set_world_map(t_map *map, int height, int width)
{
	map->world_map = malloc(sizeof(int *) * height);
	if (!map->world_map)
		return ;
	map->map_height = height;
	map->map_width = width;
}

t_map	*init_map(int height, int width)
{
	t_map	*map;
	int		y;

	map = malloc(sizeof(t_map));
	if (!map)
		return (NULL);
	ft_memset(map, 0, sizeof(t_map));
	set_world_map(map, height, width);
	if (!map->world_map)
		return (free(map), NULL);
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
			return (NULL);
		}
		y++;
	}
	return (map);
}

// t_map	*init_map(int height, int width)
// {
// 	t_map	*map;
// 	int		y;

// 	map = malloc(sizeof(t_map));
// 	if (!map)
// 		return (NULL);
// 	ft_memset(map, 0, sizeof(t_map));
// 	map->map_height = height;
// 	map->map_width = width;
// 	map->world_map = malloc(sizeof(int *) * height);
// 	if (!map->world_map)
// 		return (free(map), NULL);
// 	y = 0;
// 	while (y < height)
// 	{
// 		map->world_map[y] = malloc(sizeof(int) * width);
// 		if (!map->world_map[y])
// 		{
// 			while (--y >= 0)
// 				free(map->world_map[y]);
// 			free(map->world_map);
// 			free(map);
// 			return (NULL);
// 		}
// 		y++;
// 	}
// 	return (map);
// }
// t_map	*init_map(t_window *w)
// {
// 	t_map	*map;

// 	map = malloc(sizeof(t_map));
// 	if (!map)
// 		return (NULL);
// 	int	templatemap[MAPHEIGHT][MAPWIDTH]=
// 	{
// 	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
// 	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,0,1,0,1,0,0,0,1},
// 	{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1},
// 	{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,0,1,0,1,0,0,0,1},
// 	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
// 	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0}
// 	};
// 	for (int i = 0; i < MAPHEIGHT; i++)
// 	{
// 		for (int j = 0; j < MAPWIDTH; j++)
// 			map->world_map[i][j] = templatemap[i][j];
// 	}
// 	map->n_xpm_path = ft_strdup("./textures/north_texture.xpm");
// 	map->s_xpm_path = ft_strdup("./textures/south_texture.xpm");
// 	map->e_xpm_path = ft_strdup("./textures/east_texture.xpm");
// 	map->w_xpm_path = ft_strdup("./textures/west_texture.xpm");
// 	if (!extract_images(map, w))
// 		return (NULL);
// 	map->ceiling_color = 0x87CEEB;
// 	map->floor_color = 0x404040;
// 	map->start_position_x = 22.0;
// 	map->start_position_y = 12.0;
// 	map->start_direction = 0;
// 	return (map);
// }

void	cleanup_map(t_map *map, t_window *w)
{
	if (map->texture[0])
		mlx_destroy_image(w->mlx, map->texture[0]);
	if (map->texture[1])
		mlx_destroy_image(w->mlx, map->texture[1]);
	if (map->texture[3])
		mlx_destroy_image(w->mlx, map->texture[3]);
	if (map->texture[2])
		mlx_destroy_image(w->mlx, map->texture[2]);
	if (map->n_xpm_path)
		free(map->n_xpm_path);
	if (map->s_xpm_path)
		free(map->s_xpm_path);
	if (map->e_xpm_path)
		free(map->e_xpm_path);
	if (map->w_xpm_path)
		free(map->w_xpm_path);
}
