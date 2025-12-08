/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_maps_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:27:02 by rceliows          #+#    #+#             */
/*   Updated: 2025/12/07 16:27:04 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	load_animated_frames(t_map *map, t_window *w)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		map->animated_sprite_frames[i] = mlx_xpm_file_to_image(w->mlx,
				map->animated_sprite_path[i], &map->animated_sprite_width[i],
				&map->animated_sprite_height[i]);
		if (!map->animated_sprite_frames[i])
			return (0);
		i++;
	}
	return (1);
}

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
	if (!load_animated_frames(map, w))
		return (0);
	return (1);
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
