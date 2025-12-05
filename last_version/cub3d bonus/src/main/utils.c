/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 15:09:04 by rceliows          #+#    #+#             */
/*   Updated: 2025/10/28 15:09:05 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static void	cleanup_game_if_needed(t_cub3d *cub3d)
{
	if (cub3d->game)
	{
		clenup_game(cub3d->game);
		cub3d->game = NULL;
	}
}

static void	cleanup_map_if_needed(t_cub3d *cub3d)
{
	if (cub3d->map)
	{
		cleanup_map(cub3d->map, cub3d->window);
		free_world_map(cub3d->map);
		free(cub3d->map);
		cub3d->map = NULL;
	}
}

static void	cleanup_window_if_needed(t_cub3d *cub3d)
{
	if (cub3d->window)
	{
		cleanup_window(cub3d->window);
		free(cub3d->window);
		cub3d->window = NULL;
	}
}

static void	cleanup_other_structs(t_cub3d *cub3d)
{
	if (cub3d->raycaster)
	{
		free(cub3d->raycaster);
		cub3d->raycaster = NULL;
	}
	if (cub3d->keys)
	{
		free(cub3d->keys);
		cub3d->keys = NULL;
	}
}

void	cleanup_cub3d(t_cub3d *cub3d)
{
	if (!cub3d)
		return ;
	cleanup_game_if_needed(cub3d);
	cleanup_map_if_needed(cub3d);
	cleanup_other_structs(cub3d);
	cleanup_window_if_needed(cub3d);
	cleanup_doors();
	free(cub3d);
}

void	error_exit(t_cub3d *cub3d)
{
	printf("Error: Initialization failed\n");
	cleanup_cub3d(cub3d);
	exit(1);
}