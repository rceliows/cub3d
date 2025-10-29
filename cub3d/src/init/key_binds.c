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

int	key_press(int keycode, t_raycaster *r, t_cub3d *cub3d)
{
	if (keycode == 119)
		r->keys.w = 1;
	else if (keycode == 97)
		r->keys.a = 1;
	else if (keycode == 115)
		r->keys.s = 1;
	else if (keycode == 100)
		r->keys.d = 1;
	else if (keycode == 65361)
		r->keys.left = 1;
	else if (keycode == 65363)
		r->keys.right = 1;
	else if (keycode == 65307)
	{
		printf("ESC pressed, exiting...\n");
		cleanup_cub3d(cub3d);
		exit(0);
	}
	return (0);
}

int	key_release(int keycode, t_raycaster *r)
{
	if (keycode == 119)
		r->keys.w = 0;
	else if (keycode == 97)
		r->keys.a = 0;
	else if (keycode == 115)
		r->keys.s = 0;
	else if (keycode == 100)
		r->keys.d = 0;
	else if (keycode == 65361)
		r->keys.left = 0;
	else if (keycode == 65363)
		r->keys.right = 0;
	return (0);
}

static void	apply_rotation(t_raycaster *r, double angle)
{
	double	olddirx;
	double	oldplanex;

	olddirx = r->dirX;
	r->dirX = r->dirX * cos(angle) - r->dirY * sin(angle);
	r->dirY = olddirx * sin(angle) + r->dirY * cos(angle);
	oldplanex = r->planeX;
	r->planeX = r->planeX * cos(angle) - r->planeY * sin(angle);
	r->planeY = oldplanex * sin(angle) + r->planeY * cos(angle);
}

static void	apply_forward(t_raycaster *r, t_map *map)
{
	if (map->worldMap[(int)(r->posX + r->dirX * r->moveSpeed)]
		[(int)(r->posY)] == 0)
		r->posX += r->dirX * r->moveSpeed;
	if (map->worldMap[(int)(r->posX)]
		[(int)(r->posY + r->dirY * r->moveSpeed)] == 0)
		r->posY += r->dirY * r->moveSpeed;
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

int	mouse_move(int x, int y, t_raycaster *r, t_window *w)
{
	int		delta_x;
	double	rotation;

	(void)y;
	delta_x = x - w->center_x;
	if (delta_x != 0)
	{
		rotation = delta_x * 0.002;
		apply_rotation(r, rotation);
		mlx_mouse_move(w->mlx, w->win, w->center_x, w->center_y);
	}
	return (0);
}

void	process_movement(t_raycaster *r, t_map *map)
{
	if (r->keys.w)
		apply_forward(r, map);
	if (r->keys.s)
		apply_backward(r, map);
	if (r->keys.a)
		apply_strafe_left(r, map);
	if (r->keys.d)
		apply_strafe_right(r, map);
	if (r->keys.right)
		apply_rotation(r, r->rotSpeed);
	if (r->keys.left)
		apply_rotation(r, -r->rotSpeed);
}

void	prep_hooks(t_raycaster *r, t_window *w)
{
	mlx_hook(w->win, 2, 1L << 0, key_press, r);
	mlx_hook(w->win, 3, 1L << 1, key_release, r);
	mlx_hook(w->win, 17, 1L << 17, handle_close, r);
	mlx_hook(w->win, 6, 1L << 6, mouse_move, r);
}
