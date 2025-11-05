/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sprites.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:31:55 by rceliows          #+#    #+#             */
/*   Updated: 2025/11/05 14:31:57 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	malloc_sprites(t_sprite_data *sprites)
{
	sprites->pos_x = malloc(sizeof(double) * sprites->count);
	if (!sprites->pos_x)
		return (0);
	sprites->pos_y = malloc(sizeof(double) * sprites->count);
	if (!sprites->pos_y)
		return (0);
	sprites->type = malloc(sizeof(int) * sprites->count);
	if (!sprites->type)
		return (0);
	sprites->order = malloc(sizeof(int) * sprites->count);
	if (!sprites->order)
		return (0);
	sprites->distance = malloc(sizeof(double) * sprites->count);
	if (!sprites->distance)
		return (0);
	return (1);
}

static void	init_sprites(t_map *map, t_raycaster *r, t_sprite_data *sprites, int *idx)
{
	int	i;
	int	j;

	i = 0;
	while (i < MAPHEIGHT)
	{
		j = 0;
		while (j < MAPWIDTH)
		{
			if (map->world_map[i][j] == 3 || map->world_map[i][j] == 4)
			{
				sprites->pos_x[*idx] = i + 0.5;
				sprites->pos_y[*idx] = j + 0.5;
				sprites->type[*idx] = map->world_map[i][j];
				sprites->order[*idx] = *idx;
				sprites->distance[*idx] = ((r->pos_x - sprites->pos_x[*idx])
					* (r->pos_x - sprites->pos_x[*idx])
					+ (r->pos_y - sprites->pos_y[*idx])
					* (r->pos_y - sprites->pos_y[*idx]));
				(*idx)++;
			}
			j++;
		}
		i++;
	}
}

int	collect_sprites(t_map *map, t_raycaster *r, t_sprite_data *sprites)
{
	int	i;
	int	j;
	int	idx;

	sprites->count = 0;
	i = 0;
	while (i < MAPHEIGHT)
	{
		j = 0;
		while (j < MAPWIDTH)
		{
			if (map->world_map[i][j] == 3 || map->world_map[i][j] == 4)
				sprites->count++;
			j++;
		}
		i++;
	}
	if (!!malloc_sprites(sprites))
		return (0);
	idx = 0;
	init_sprites(map, r, sprites, &idx);
	return (1);
}


void	cleanup_sprites(t_sprite_data *sprites)
{
	if (sprites->pos_x)
		free(sprites->pos_x);
	if (sprites->pos_y)
		free(sprites->pos_y);
	if (sprites->type)
		free(sprites->type);
	if (sprites->order)
		free(sprites->order);
	if (sprites->distance)
		free(sprites->distance);
}
