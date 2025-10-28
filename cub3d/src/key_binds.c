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

int	key_press(int keycode, t_raycaster *raycaster)
{
	if (keycode == 119)
		raycaster->keys.w = 1;
	else if (keycode == 97)
		raycaster->keys.a = 1;
	else if (keycode == 115)
		raycaster->keys.s = 1;
	else if (keycode == 100)
		raycaster->keys.d = 1;
	else if (keycode == 65361)
		raycaster->keys.left = 1;
	else if (keycode == 65363)
		raycaster->keys.right = 1;
	else if (keycode == 65307)
	{
		printf("ESC pressed, exiting...\n");
		cleanup_raycaster(raycaster);
		exit(0);
	}
	return (0);
}

int	key_release(int keycode, t_raycaster *raycaster)
{
	if (keycode == 119)
		raycaster->keys.w = 0;
	else if (keycode == 97)
		raycaster->keys.a = 0;
	else if (keycode == 115)
		raycaster->keys.s = 0;
	else if (keycode == 100)
		raycaster->keys.d = 0;
	else if (keycode == 65361)
		raycaster->keys.left = 0;
	else if (keycode == 65363)
		raycaster->keys.right = 0;
	return (0);
}

static void	apply_rotation(t_raycaster *r, double angle)
{
	double	oldDirX;
	double	oldPlaneX;

	oldDirX = r->dirX;
	r->dirX = r->dirX * cos(angle) - r->dirY * sin(angle);
	r->dirY = oldDirX * sin(angle) + r->dirY * cos(angle);
	oldPlaneX = r->planeX;
	r->planeX = r->planeX * cos(angle) - r->planeY * sin(angle);
	r->planeY = oldPlaneX * sin(angle) + r->planeY * cos(angle);
}

static void	apply_forward(t_raycaster *r)
{
	if (worldMap[(int)(r->posX + r->dirX * r->moveSpeed)]
		[(int)(r->posY)] == 0)
		r->posX += r->dirX * r->moveSpeed;
	if (worldMap[(int)(r->posX)]
		[(int)(r->posY + r->dirY * r->moveSpeed)] == 0)
		r->posY += r->dirY * r->moveSpeed;
}

static void	apply_backward(t_raycaster *r)
{
	if (worldMap[(int)(r->posX - r->dirX * r->moveSpeed)]
		[(int)(r->posY)] == 0)
		r->posX -= r->dirX * r->moveSpeed;
	if (worldMap[(int)(r->posX)]
		[(int)(r->posY - r->dirY * r->moveSpeed)] == 0)
		r->posY -= r->dirY * r->moveSpeed;
}

static void	apply_strafe_right(t_raycaster *r)
{
	if (worldMap[(int)(r->posX - r->dirY * r->moveSpeed)]
		[(int)(r->posY)] == 0)
		r->posX -= r->dirY * r->moveSpeed;
	if (worldMap[(int)(r->posX)]
		[(int)(r->posY + r->dirX * r->moveSpeed)] == 0)
		r->posY += r->dirX * r->moveSpeed;
}

static void	apply_strafe_left(t_raycaster *r)
{
	if (worldMap[(int)(r->posX + r->dirY * r->moveSpeed)]
		[(int)(r->posY)] == 0)
		r->posX += r->dirY * r->moveSpeed;
	if (worldMap[(int)(r->posX)]
		[(int)(r->posY - r->dirX * r->moveSpeed)] == 0)
		r->posY -= r->dirX * r->moveSpeed;
}

int	mouse_move(int x, int y, t_raycaster *raycaster)
{
	int		delta_x;
	double	rotation;

	(void)y;
	if (!raycaster->mouse_init)
	{
		raycaster->mouse_x = x;
		raycaster->mouse_init = 1;
		return (0);
	}
	delta_x = x - raycaster->mouse_x;
	raycaster->mouse_x = x;
	if (delta_x != 0)
	{
		rotation = delta_x * 0.002;
		apply_rotation(raycaster, rotation);
	}
	return (0);
}

void	process_movement(t_raycaster *raycaster)
{
	if (raycaster->keys.w)
		apply_forward(raycaster);
	if (raycaster->keys.s)
		apply_backward(raycaster);
	if (raycaster->keys.a)
		apply_strafe_left(raycaster);
	if (raycaster->keys.d)
		apply_strafe_right(raycaster);
	if (raycaster->keys.right)
		apply_rotation(raycaster, raycaster->rotSpeed);
	if (raycaster->keys.left)
		apply_rotation(raycaster, -raycaster->rotSpeed);
}

void	prep_hooks(t_raycaster *raycaster)
{
	mlx_hook(raycaster->win, 2, 1L << 0, key_press, raycaster);
	mlx_hook(raycaster->win, 3, 1L << 1, key_release, raycaster);
	mlx_hook(raycaster->win, 17, 1L << 17, handle_close, raycaster);
	mlx_hook(raycaster->win, 6, 1L << 6, mouse_move, raycaster);
}