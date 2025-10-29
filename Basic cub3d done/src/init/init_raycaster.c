/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_raycaster.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 16:38:37 by rceliows          #+#    #+#             */
/*   Updated: 2025/10/29 16:38:38 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	setup_direction(t_raycaster *raycaster, int direction)
{
	if (direction == 'N')
	{
		raycaster->dirX = 0;
		raycaster->dirY = -1;
	}
	else if (direction == 'S')
	{
		raycaster->dirX = 0;
		raycaster->dirY = 1;
	}
	else if (direction == 'E')
	{
		raycaster->dirX = 1;
		raycaster->dirY = 0;
	}
	else if (direction == 'W')
	{
		raycaster->dirX = -1;
		raycaster->dirY = 0;
	}
}

void	init_raycaster(t_raycaster *raycaster, t_window *window, int direction)
{
	struct timeval tv;

	raycaster->img_data = mlx_get_data_addr(window->img, 
		&raycaster->bits_per_pixel, &raycaster->line_length, &raycaster->endian);
	raycaster->posX = 22.0;
	raycaster->posY = 12.0;
	setup_direction(raycaster, direction);
	raycaster->planeX = -raycaster->dirY * 0.66;
	raycaster->planeY = raycaster->dirX * 0.66;
	raycaster->screenScale = sqrt((raycaster->screenWidth * raycaster->screenHeight) / (1920.0 * 1080.0));
	raycaster->baseMovespeed = 1.0 * raycaster->screenScale;
	raycaster->baseRotSpeed = 0.3 * raycaster->screenScale;
	gettimeofday(&tv, NULL);
	raycaster->lastTime = tv.tv_sec + tv.tv_usec / 1000000.0;
	raycaster->frameTime = 0.016;
	raycaster->moveSpeed = raycaster->baseMovespeed * raycaster->frameTime;
	raycaster->rotSpeed = raycaster->baseRotSpeed * raycaster->frameTime;
	raycaster->oldTime = 0;
}