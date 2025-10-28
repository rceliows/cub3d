/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_binds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 15:12:00 by rceliows          #+#    #+#             */
/*   Updated: 2025/10/28 15:12:01 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	handle_close(t_raycaster *raycaster)
{
	printf("Cleaning and exiting...\n");
	cleanup_raycaster(raycaster);
	exit(0);
	return (0);
}

void	handle_rotation_right(t_raycaster *raycaster)
{
	double	oldDirX;
	double	oldPlaneX;

	oldDirX = raycaster->dirX;
	raycaster->dirX = raycaster->dirX * cos(-raycaster->rotSpeed) 
		- raycaster->dirY * sin(-raycaster->rotSpeed);
	raycaster->dirY = oldDirX * sin(-raycaster->rotSpeed) 
		+ raycaster->dirY * cos(-raycaster->rotSpeed);
	oldPlaneX = raycaster->planeX;
	raycaster->planeX = raycaster->planeX * cos(-raycaster->rotSpeed) 
		- raycaster->planeY * sin(-raycaster->rotSpeed);
	raycaster->planeY = oldPlaneX * sin(-raycaster->rotSpeed) 
		+ raycaster->planeY * cos(-raycaster->rotSpeed);
}

void	handle_rotation_left(t_raycaster *raycaster)
{
	double	oldDirX;
	double	oldPlaneX;

	oldDirX = raycaster->dirX;
	raycaster->dirX = raycaster->dirX * cos(raycaster->rotSpeed) 
		- raycaster->dirY * sin(raycaster->rotSpeed);
	raycaster->dirY = oldDirX * sin(raycaster->rotSpeed) 
		+ raycaster->dirY * cos(raycaster->rotSpeed);
	oldPlaneX = raycaster->planeX;
	raycaster->planeX = raycaster->planeX * cos(raycaster->rotSpeed) 
		- raycaster->planeY * sin(raycaster->rotSpeed);
	raycaster->planeY = oldPlaneX * sin(raycaster->rotSpeed) 
		+ raycaster->planeY * cos(raycaster->rotSpeed);
}

void	handle_movement_right(t_raycaster *raycaster)
{
	if (worldMap[(int)(raycaster->posX - raycaster->dirY * raycaster->moveSpeed)]
		[(int)(raycaster->posY)] == 0)
		raycaster->posX -= raycaster->dirY * raycaster->moveSpeed;
	if (worldMap[(int)(raycaster->posX)]
		[(int)(raycaster->posY + raycaster->dirX * raycaster->moveSpeed)] == 0)
		raycaster->posY += raycaster->dirX * raycaster->moveSpeed;
}

void	handle_movement_left(t_raycaster *raycaster)
{
	if (worldMap[(int)(raycaster->posX + raycaster->dirY * raycaster->moveSpeed)]
		[(int)(raycaster->posY)] == 0)
		raycaster->posX += raycaster->dirY * raycaster->moveSpeed;
	if (worldMap[(int)(raycaster->posX)]
		[(int)(raycaster->posY - raycaster->dirX * raycaster->moveSpeed)] == 0)
		raycaster->posY -= raycaster->dirX * raycaster->moveSpeed;
}

void	handle_movement_forward(t_raycaster *raycaster)
{
	if (worldMap[(int)(raycaster->posX + raycaster->dirX * raycaster->moveSpeed)]
		[(int)(raycaster->posY)] == 0)
		raycaster->posX += raycaster->dirX * raycaster->moveSpeed;
	if (worldMap[(int)(raycaster->posX)]
		[(int)(raycaster->posY + raycaster->dirY * raycaster->moveSpeed)] == 0)
		raycaster->posY += raycaster->dirY * raycaster->moveSpeed;
}

void	handle_movement_backward(t_raycaster *raycaster)
{
	if (worldMap[(int)(raycaster->posX - raycaster->dirX * raycaster->moveSpeed)]
		[(int)(raycaster->posY)] == 0)
		raycaster->posX -= raycaster->dirX * raycaster->moveSpeed;
	if (worldMap[(int)(raycaster->posX)]
		[(int)(raycaster->posY - raycaster->dirY * raycaster->moveSpeed)] == 0)
		raycaster->posY -= raycaster->dirY * raycaster->moveSpeed;
}

int	key_dispatcher(int keycode, t_raycaster *raycaster)
{
	if (keycode == 65363)
		handle_rotation_right(raycaster);
	else if (keycode == 65361)
		handle_rotation_left(raycaster);
	else if (keycode == 100)
		handle_movement_right(raycaster);
	else if (keycode == 97)
		handle_movement_left(raycaster);
	else if (keycode == 119)
		handle_movement_forward(raycaster);
	else if (keycode == 115)
		handle_movement_backward(raycaster);
	else if (keycode == 65307)
	{
		printf("ESC pressed, exiting...\n");
		cleanup_raycaster(raycaster);
		exit(0);
	}
	return (0);
}

void	prep_hooks(t_raycaster *raycaster)
{
	mlx_hook(raycaster->win, 2, 1L << 0, key_dispatcher, raycaster);
	mlx_hook(raycaster->win, 17, 1L << 17, handle_close, raycaster);
}