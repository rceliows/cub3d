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

void	init_raycaster(t_raycaster *raycaster)
{
	struct timeval tv;
	
	raycaster->mlx = mlx_init();
	if (!raycaster->mlx)
		error_exit(raycaster);
	mlx_get_screen_size(raycaster->mlx, &raycaster->screenWidth, &raycaster->screenHeight);
	raycaster->win = mlx_new_window(raycaster->mlx, raycaster->screenWidth, raycaster->screenHeight, "cub3d");
	if (!raycaster->win)
		error_exit(raycaster);
	raycaster->img = mlx_new_image(raycaster->mlx, raycaster->screenWidth, raycaster->screenHeight);
	if (!raycaster->img)
		error_exit(raycaster);
	raycaster->img_data = mlx_get_data_addr(raycaster->img, 
		&raycaster->bits_per_pixel, &raycaster->line_length, &raycaster->endian);
	raycaster->posX = 22.0;
	raycaster->posY = 12.0;
	raycaster->dirX = raycaster->start_posX;
	raycaster->dirY = raycaster->start_posY;
	raycaster->planeX = -raycaster->start_posY * 0.66;
	raycaster->planeY = raycaster->start_posX * 0.66;
	raycaster->screenScale = sqrt((raycaster->screenWidth * raycaster->screenHeight) / (1920.0 * 1080.0));
	raycaster->baseMovespeed = 1.0 * raycaster->screenScale;
	raycaster->baseRotSpeed = 0.3 * raycaster->screenScale;
	gettimeofday(&tv, NULL);
	raycaster->lastTime = tv.tv_sec + tv.tv_usec / 1000000.0;
	raycaster->frameTime = 0.016;
	raycaster->moveSpeed = raycaster->baseMovespeed * raycaster->frameTime;
	raycaster->rotSpeed = raycaster->baseRotSpeed * raycaster->frameTime;
	raycaster->oldTime = 0;
	raycaster->keys.w = 0;
	raycaster->keys.a = 0;
	raycaster->keys.s = 0;
	raycaster->keys.d = 0;
	raycaster->keys.left = 0;
	raycaster->keys.right = 0;
	raycaster->ceiling_color = 0x87CEEB;
	raycaster->floor_color = 0x404040;
}

void	cleanup_raycaster(t_raycaster *raycaster)
{
	if (raycaster->img)
	{
		mlx_destroy_image(raycaster->mlx, raycaster->img);
		raycaster->img = NULL;
	}
	if (raycaster->win)
	{
		mlx_destroy_window(raycaster->mlx, raycaster->win);
		raycaster->win = NULL;
	}
	if (raycaster->mlx)
	{
		mlx_destroy_display(raycaster->mlx);
		free(raycaster->mlx);
		raycaster->mlx = NULL;
	}
}

void	error_exit(t_raycaster *raycaster)
{
	cleanup_raycaster(raycaster);
	exit(1);
}