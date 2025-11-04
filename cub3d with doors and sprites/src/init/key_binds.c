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

static int	key_press_bonus(int keycode, t_raycaster *r)
{
	if (keycode == 102)
		r->keys.fps = !r->keys.fps;
	else if (keycode == 109)
		r->keys.minimap = !r->keys.minimap;
	else
		return (0);
	return (1);
}

static int	key_press_movement(int keycode, t_raycaster *r)
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
	else
		return (0);
	return (1);
}

int	key_press(int keycode, t_cub3d *cub3d)
{
	t_raycaster	*r;

	r = cub3d->raycaster;
	if (key_press_bonus(keycode, r))
		return (0);
	if (key_press_movement(keycode, r))
		return (0);
	if (keycode == 65307)
	{
		printf("ESC pressed, exiting...\n");
		cleanup_cub3d(cub3d);
		exit(0);
	}
	return (0);
}

static int	key_release_movement(int keycode, t_raycaster *r)
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
	else
		return (0);
	return (1);
}

int	key_release(int keycode, t_cub3d *cub3d)
{
	t_raycaster	*r;

	r = cub3d->raycaster;
	key_release_movement(keycode, r);
	return (0);
}

static void	apply_rotation(t_raycaster *r, double angle)
{
	double	olddir_x;
	double	oldplane_x;

	olddir_x = r->dir_x;
	r->dir_x = r->dir_x * cos(angle) - r->dir_y * sin(angle);
	r->dir_y = olddir_x * sin(angle) + r->dir_y * cos(angle);
	oldplane_x = r->plane_x;
	r->plane_x = r->plane_x * cos(angle) - r->plane_y * sin(angle);
	r->plane_y = oldplane_x * sin(angle) + r->plane_y * cos(angle);
}

static void	apply_forward(t_raycaster *r, t_map *map)
{
	if (map->world_map[(int)(r->pos_x + r->dir_x * r->move_speed)]
		[(int)(r->pos_y)] == 0)
		r->pos_x += r->dir_x * r->move_speed;
	if (map->world_map[(int)(r->pos_x)]
		[(int)(r->pos_y + r->dir_y * r->move_speed)] == 0)
		r->pos_y += r->dir_y * r->move_speed;
}

static void	apply_backward(t_raycaster *r, t_map *map)
{
	if (map->world_map[(int)(r->pos_x - r->dir_x * r->move_speed)]
		[(int)(r->pos_y)] == 0)
		r->pos_x -= r->dir_x * r->move_speed;
	if (map->world_map[(int)(r->pos_x)]
		[(int)(r->pos_y - r->dir_y * r->move_speed)] == 0)
		r->pos_y -= r->dir_y * r->move_speed;
}

static void	apply_strafe_right(t_raycaster *r, t_map *map)
{
	if (map->world_map[(int)(r->pos_x - r->dir_y * r->move_speed)]
		[(int)(r->pos_y)] == 0)
		r->pos_x -= r->dir_y * r->move_speed;
	if (map->world_map[(int)(r->pos_x)]
		[(int)(r->pos_y + r->dir_x * r->move_speed)] == 0)
		r->pos_y += r->dir_x * r->move_speed;
}

static void	apply_strafe_left(t_raycaster *r, t_map *map)
{
	if (map->world_map[(int)(r->pos_x + r->dir_y * r->move_speed)]
		[(int)(r->pos_y)] == 0)
		r->pos_x += r->dir_y * r->move_speed;
	if (map->world_map[(int)(r->pos_x)]
		[(int)(r->pos_y - r->dir_x * r->move_speed)] == 0)
		r->pos_y -= r->dir_x * r->move_speed;
}

static void	mouse_position_reset(int delta_x, int delta_y, t_window *w)
{
	if (delta_x != 0 || delta_y != 0)
		mlx_mouse_move(w->mlx, w->win, w->center_x, w->center_y);
}

int	mouse_move(int x, int y, t_cub3d *cub3d)
{
	int			delta_x;
	int			delta_y;
	double		rotation;
	t_raycaster	*r;
	t_window	*w;

	r = cub3d->raycaster;
	w = cub3d->window;
	delta_x = x - w->center_x;
	if (delta_x != 0)
	{
		rotation = delta_x * 0.002;
		apply_rotation(r, rotation);
	}
	delta_y = y - w->center_y;
	if (delta_y != 0)
	{
		r->pitch -= delta_y;
		if (r->pitch > DEFSCREENHEIGHT / 2)
			r->pitch = DEFSCREENHEIGHT / 2;
		if (r->pitch < -DEFSCREENHEIGHT / 2)
			r->pitch = -DEFSCREENHEIGHT / 2;
	}
	mouse_position_reset(delta_x, delta_y, w);
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
		apply_rotation(r, r->rot_speed);
	if (r->keys.left)
		apply_rotation(r, -r->rot_speed);
}

void	prep_hooks(t_cub3d *cub3d)
{
	mlx_hook(cub3d->window->win, 2, 1L << 0, key_press, cub3d);
	mlx_hook(cub3d->window->win, 3, 1L << 1, key_release, cub3d);
	mlx_hook(cub3d->window->win, 17, 1L << 17, handle_close, cub3d);
	mlx_hook(cub3d->window->win, 6, 1L << 6, mouse_move, cub3d);
}
