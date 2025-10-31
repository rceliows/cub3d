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
	{
		free(cub3d->raycaster);
		cub3d->raycaster = NULL;
	}
	if (cub3d->keys)
	{
		free(cub3d->keys);
		cub3d->keys = NULL;
	}
		if (cub3d->window)
	{
		cleanup_window(cub3d->window);
		free(cub3d->window);
		cub3d->window = NULL;
	}
	free(cub3d);
}

void	error_exit(t_cub3d *cub3d)
{
	printf("Error: Initialization failed\n");
	cleanup_cub3d(cub3d);
	exit(1);
}
