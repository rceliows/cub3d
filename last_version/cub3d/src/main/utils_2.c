/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:25:49 by rceliows          #+#    #+#             */
/*   Updated: 2025/12/07 16:25:50 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

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
