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

int	handle_close(t_cub3d *cub3d)
{
	printf("Cleaning and exiting...\n");
	cleanup_cub3d(cub3d);
	exit(0);
	return (0);
}

int	key_press(int keycode, t_raycaster *raycaster, t_cub3d *cub3d)
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
		cleanup_cub3d(cub3d);
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

static void	apply_rotation(t_raycaster *raycaster, double angle)
{
	double	oldDirX;
	double	oldPlaneX;

	oldDirX = raycaster->dirX;
	raycaster->dirX = raycaster->dirX * cos(angle) - raycaster->dirY * sin(angle);
	raycaster->dirY = oldDirX * sin(angle) + raycaster->dirY * cos(angle);
	oldPlaneX = raycaster->planeX;
	raycaster->planeX = raycaster->planeX * cos(angle) - raycaster->planeY * sin(angle);
	raycaster->planeY = oldPlaneX * sin(angle) + raycaster->planeY * cos(angle);
}

static void	apply_forward(t_raycaster *raycaster, t_map *map)
{
	if (map->worldMap[(int)(raycaster->posX + raycaster->dirX * raycaster->moveSpeed)]
		[(int)(raycaster->posY)] == 0)
		raycaster->posX += raycaster->dirX * raycaster->moveSpeed;
	if (map->worldMap[(int)(raycaster->posX)]
		[(int)(raycaster->posY + raycaster->dirY * raycaster->moveSpeed)] == 0)
		raycaster->posY += raycaster->dirY * raycaster->moveSpeed;
}

static void	apply_backward(t_raycaster *r, t_map *map)
{
	if (map->worldMap[(int)(r->posX - r->dirX * r->moveSpeed)]
		[(int)(r->posY)] == 0)
		r->posX -= r->dirX * r->moveSpeed;
	if (map->worldMap[(int)(r->posX)]
		[(int)(r->posY - r->dirY * r->moveSpeed)] == 0)
		r->posY -= r->dirY * r->moveSpeed;
}

static void	apply_strafe_right(t_raycaster *r, t_map *map)
{
	if (map->worldMap[(int)(r->posX - r->dirY * r->moveSpeed)]
		[(int)(r->posY)] == 0)
		r->posX -= r->dirY * r->moveSpeed;
	if (map->worldMap[(int)(r->posX)]
		[(int)(r->posY + r->dirX * r->moveSpeed)] == 0)
		r->posY += r->dirX * r->moveSpeed;
}

static void	apply_strafe_left(t_raycaster *r, t_map *map)
{
	if (map->worldMap[(int)(r->posX + r->dirY * r->moveSpeed)]
		[(int)(r->posY)] == 0)
		r->posX += r->dirY * r->moveSpeed;
	if (map->worldMap[(int)(r->posX)]
		[(int)(r->posY - r->dirX * r->moveSpeed)] == 0)
		r->posY -= r->dirX * r->moveSpeed;
}

int	mouse_move(int x, int y, t_raycaster *raycaster, t_window *window)
{
	int		center_x;
	int		center_y;
	int		delta_x;
	double	rotation;

	center_x = raycaster->screenWidth / 2;
	center_y = raycaster->screenHeight / 2;
	(void)y;
	delta_x = x - center_x;
	if (delta_x != 0)
	{
		rotation = delta_x * 0.002;
		apply_rotation(raycaster, rotation);
		mlx_mouse_move(window->mlx, window->win, center_x, center_y);
	}
	return (0);
}

void	process_movement(t_raycaster *raycaster, t_map *map)
{
	if (raycaster->keys.w)
		apply_forward(raycaster, map);
	if (raycaster->keys.s)
		apply_backward(raycaster, map);
	if (raycaster->keys.a)
		apply_strafe_left(raycaster, map);
	if (raycaster->keys.d)
		apply_strafe_right(raycaster, map);
	if (raycaster->keys.right)
		apply_rotation(raycaster, raycaster->rotSpeed);
	if (raycaster->keys.left)
		apply_rotation(raycaster, -raycaster->rotSpeed);
}

void	prep_hooks(t_raycaster *raycaster, t_window *window)
{
	mlx_hook(window->win, 2, 1L << 0, key_press, raycaster);
	mlx_hook(window->win, 3, 1L << 1, key_release, raycaster);
	mlx_hook(window->win, 17, 1L << 17, handle_close, raycaster);
	mlx_hook(window->win, 6, 1L << 6, mouse_move, raycaster);
}