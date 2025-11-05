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

static void	cleanup_raycaster(t_raycaster *r)
{
	free(r);
	r = NULL;
}

static void	cleanup_keys(t_keys *k)
{
	free(k);
	k = NULL;
}

void	cleanup_cub3d(t_cub3d *cub3d)
{
	if (!cub3d)
		return ;
	if (cub3d->map)
	{
		cleanup_map(cub3d->map, cub3d->window);
		free(cub3d->map);
		cub3d->map = NULL;
	}
	if (cub3d->raycaster)
		cleanup_raycaster(cub3d->raycaster);
	if (cub3d->keys)
		cleanup_keys(cub3d->keys);
	if (cub3d->window)
	{
		cleanup_window(cub3d->window);
		free(cub3d->window);
		cub3d->window = NULL;
	}
	cleanup_doors();
	free(cub3d);
}

void	error_exit(t_cub3d *cub3d)
{
	printf("Error: Initialization failed\n");
	cleanup_cub3d(cub3d);
	exit(1);
}
