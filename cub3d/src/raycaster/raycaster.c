/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 15:09:26 by rceliows          #+#    #+#             */
/*   Updated: 2025/10/28 15:09:28 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

#define RED   0xFF0000
#define GREEN 0x00FF00
#define BLUE  0x0000FF
#define WHITE 0xFFFFFF
#define YELLOW 0xFFFF00

int	get_wall_color(int direction)
{
	int	color;

	if (direction == 1)
		color = RED;
	else if (direction == 2)
		color = GREEN;
	else if (direction == 3)
		color = BLUE;
	else if (direction == 4)
		color = WHITE;
	else
		color = YELLOW;
	return (color);
}

void	display_fps(t_raycaster *r, t_window *w)
{
	static double	fps_timer = 0;
	static int		frame_count = 0;
	static double	current_fps = 0;
	char			fps_str[50];

	fps_timer += r->frame_time;
	frame_count++;
	if (fps_timer >= 1.0)
	{
		current_fps = frame_count / fps_timer;
		frame_count = 0;
		fps_timer = 0;
	}
	sprintf(fps_str, "FPS: %.1f", current_fps);
	mlx_string_put(w->mlx, w->win, 10, 20, 0xFFFFFF, fps_str);
}

static void	draw_column(t_raycaster *r, int x,
				t_raycaster_var *v, int wallColor)
{
	char			*pixel_ptr;
	int				pixel_size;
	int				y;

	pixel_size = r->bits_per_pixel / 8;
	pixel_ptr = r->img_data + x * pixel_size;
	y = 0;
	while (y < v->draw_start)
	{
		*(unsigned int *)pixel_ptr = r->ceiling_color;
		pixel_ptr += r->line_length;
		y++;
	}
	while (y <= v->draw_end)
	{
		*(unsigned int *)pixel_ptr = wallColor;
		pixel_ptr += r->line_length;
		y++;
	}
	while (y < DEFSCREENHEIGHT)
	{
		*(unsigned int *)pixel_ptr = r->floor_color;
		pixel_ptr += r->line_length;
		y++;
	}
}

static void	update_time_and_speed(t_raycaster *r)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	r->current_time = tv.tv_sec + tv.tv_usec / 1000000.0;
	r->frame_time = r->current_time - r->last_time;
	r->last_time = r->current_time;
	if (r->frame_time > 0.1)
		r->frame_time = 0.1;
	r->move_speed = r->base_move_speed * r->frame_time;
	r->rot_speed = r->base_rot_speed * r->frame_time;
}

static void	perform_dda(t_map *map, t_raycaster_var *v)
{
	int	hit;

	hit = 0;
	while (hit == 0)
	{
		if (v->side_dist_x < v->side_dist_y)
		{
			v->side_dist_x += v->delta_dist_x;
			v->map_x += v->step_x;
			v->side = 0;
		}
		else
		{
			v->side_dist_y += v->delta_dist_y;
			v->map_y += v->step_y;
			v->side = 1;
		}
		if (map->world_map[v->map_x][v->map_y] > 0)
			hit = 1;
	}
}

static void	calculate_line(t_raycaster_var *v)
{
	if (v->side == 0)
		v->perp_wall_dist = (v->side_dist_x - v->delta_dist_x);
	else
		v->perp_wall_dist = (v->side_dist_y - v->delta_dist_y);
	v->line_height = (int)(DEFSCREENHEIGHT / v->perp_wall_dist);
	v->draw_start = -v->line_height / 2 + DEFSCREENHEIGHT / 2;
	if (v->draw_start < 0)
		v->draw_start = 0;
	v->draw_end = v->line_height / 2 + DEFSCREENHEIGHT / 2;
	if (v->draw_end >= DEFSCREENHEIGHT)
		v->draw_end = DEFSCREENHEIGHT - 1;
}

static void	calculate_side_distance(t_raycaster *r, t_raycaster_var *v)
{
	if (v->ray_dir_x < 0)
	{
		v->step_x = -1;
		v->side_dist_x = (r->pos_x - v->map_x) * v->delta_dist_x;
	}
	else
	{
		v->step_x = 1;
		v->side_dist_x = (v->map_x + 1.0 - r->pos_x) * v->delta_dist_x;
	}
	if (v->ray_dir_y < 0)
	{
		v->step_y = -1;
		v->side_dist_y = (r->pos_y - v->map_y) * v->delta_dist_y;
	}
	else
	{
		v->step_y = 1;
		v->side_dist_y = (v->map_y + 1.0 - r->pos_y) * v->delta_dist_y;
	}
}

static void	calculate_ray(int x, t_raycaster *r, t_raycaster_var *v)
{
	v->camera_x = 2 * x / (double)DEFSCREENWIDTH - 1;
	v->ray_dir_x = r->dir_x + r->plane_x * v->camera_x;
	v->ray_dir_y = r->dir_y + r->plane_y * v->camera_x;
	v->map_x = (int)(r->pos_x);
	v->map_y = (int)(r->pos_y);
	if (v->ray_dir_x == 0)
		v->delta_dist_x = 1e30;
	else
		v->delta_dist_x = fabs(1 / v->ray_dir_x);
	if (v->ray_dir_y == 0)
		v->delta_dist_y = 1e30;
	else
		v->delta_dist_y = fabs(1 / v->ray_dir_y);
}

static void	draw_image(int x, t_raycaster *r, t_raycaster_var *v)
{
	int		color;
	int		direction;

	if (v->side == 0)
	{
		if (v->step_x == -1)
			direction = 3;
		else
			direction = 4;
	}
	else
	{
		if (v->step_y == -1)
			direction = 1;
		else
			direction = 2;
	}
	color = get_wall_color(direction);
	draw_column(r, x, v, color);
}

int	raycasting_function(t_cub3d *cub3d)
{
	t_raycaster_var	v;
	int				x;
	t_raycaster		*r;
	t_window		*w;
	t_map			*map;

	r = cub3d->raycaster;
	w = cub3d->window;
	map = cub3d->map;
	update_time_and_speed(r);
	process_movement(r, map);
	x = 0;
	while (x < DEFSCREENWIDTH)
	{
		calculate_ray(x, r, &v);
		calculate_side_distance(r, &v);
		perform_dda(map, &v);
		calculate_line(&v);
		draw_image(x, r, &v);
		x++;
	}
	mlx_put_image_to_window(w->mlx, w->win, w->img, 0, 0);
	display_fps(r, w);
	return (0);
}
