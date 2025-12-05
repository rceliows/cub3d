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
	map->animated_sprite_frames[0] = mlx_xpm_file_to_image(w->mlx,
			map->animated_sprite_path[0], &map->animated_sprite_width[0],
			&map->animated_sprite_height[0]);
	if (!map->animated_sprite_frames[0])
		return (0);
	map->animated_sprite_frames[1] = mlx_xpm_file_to_image(w->mlx,
			map->animated_sprite_path[1], &map->animated_sprite_width[1],
			&map->animated_sprite_height[1]);
	if (!map->animated_sprite_frames[1])
		return (0);
	map->animated_sprite_frames[2] = mlx_xpm_file_to_image(w->mlx,
			map->animated_sprite_path[2], &map->animated_sprite_width[2],
			&map->animated_sprite_height[2]);
	if (!map->animated_sprite_frames[2])
		return (0);
	map->animated_sprite_frames[3] = mlx_xpm_file_to_image(w->mlx,
			map->animated_sprite_path[3], &map->animated_sprite_width[3],
			&map->animated_sprite_height[3]);
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
	cleanup_map_bonus(map, w);
}
