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

int	extract_images_bonus(t_map *map, t_window *w)
{
	map->texture[4] = mlx_xpm_file_to_image(w->mlx, map->door_path,
			&map->texture_width[4], &map->texture_height[4]);
	if (!map->texture[4])
		return (0);
	map->texture[5] = mlx_xpm_file_to_image(w->mlx, map->sprite_path,
			&map->texture_width[5], &map->texture_height[5]);
	if (!map->texture[5])
		return (0);
	map->animated_sprite_frames[0] = mlx_xpm_file_to_image(w->mlx, map->animated_sprite_path[0],
			&map->animated_sprite_width[0], &map->animated_sprite_height[0]);
	if (!map->animated_sprite_frames[0])
		return (0);
	map->animated_sprite_frames[1] = mlx_xpm_file_to_image(w->mlx, map->animated_sprite_path[1],
			&map->animated_sprite_width[1], &map->animated_sprite_height[1]);
	if (!map->animated_sprite_frames[1])
		return (0);
	map->animated_sprite_frames[2] = mlx_xpm_file_to_image(w->mlx, map->animated_sprite_path[2],
			&map->animated_sprite_width[2], &map->animated_sprite_height[2]);
	if (!map->animated_sprite_frames[2])
		return (0);
	map->animated_sprite_frames[3] = mlx_xpm_file_to_image(w->mlx, map->animated_sprite_path[3],
			&map->animated_sprite_width[3], &map->animated_sprite_height[3]);
	if (!map->animated_sprite_frames[3])
		return (0);
	return (1);
}

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

t_map	*init_map(t_window *w)
{
	t_map	*map;

	map = malloc(sizeof(t_map));
	if (!map)
		return (NULL);
	int	templatemap[MAPHEIGHT][MAPWIDTH]=
	{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,3,0,3,0,3,0,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,0,4,0,1,0,0,0,0,3,0,4,0,3,0,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,3,0,3,0,3,0,0,0,1},
	{1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,4,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,3,0,3,0,3,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,1,1,1,0,1,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,1,1,1,1,1,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	for (int i = 0; i < MAPHEIGHT; i++)
	{
		for (int j = 0; j < MAPWIDTH; j++)
			map->world_map[i][j] = templatemap[i][j];
	}
	map->n_xpm_path = ft_strdup("./textures/north_texture.xpm");
	map->s_xpm_path = ft_strdup("./textures/south_texture.xpm");
	map->e_xpm_path = ft_strdup("./textures/east_texture.xpm");
	map->w_xpm_path = ft_strdup("./textures/west_texture.xpm");
	map->door_path = ft_strdup("./textures/door.xpm");
	map->sprite_path = ft_strdup("./textures/sprite.xpm");
	map->animated_sprite_path[0] = ft_strdup("./textures/animated_sprite_frame_1.xpm");
	map->animated_sprite_path[1] = ft_strdup("./textures/animated_sprite_frame_2.xpm");
	map->animated_sprite_path[2] = ft_strdup("./textures/animated_sprite_frame_3.xpm");
	map->animated_sprite_path[3] = ft_strdup("./textures/animated_sprite_frame_2.xpm");
	if (!extract_images(map, w))
		return (NULL);
	if (!extract_images_bonus(map,w))
		return (NULL);
	map->ceiling_color = 0x87CEEB;
	map->floor_color = 0x404040;
	map->start_position_x = 22.0;
	map->start_position_y = 12.0;
	map->start_direction = 0;
	return (map);
}

static void	cleanup_map_bonus(t_map *map, t_window *w)
{
	int	i;

	if (map->texture[4])
		mlx_destroy_image(w->mlx, map->texture[4]);
	if (map->texture[5])
		mlx_destroy_image(w->mlx, map->texture[5]);
	i = 0;
	while (i < 4)
	{
		if (map->animated_sprite_frames[i])
			mlx_destroy_image(w->mlx, map->animated_sprite_frames[i]);
		if (map->animated_sprite_path[i])
			free(map->animated_sprite_path[i]);
		i++;
	}
	if (map->door_path)
		free(map->door_path);
	if (map->sprite_path)
		free(map->sprite_path);
}

void    cleanup_map(t_map *map, t_window *w)
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
	cleanup_map_bonus(map, w);
}
