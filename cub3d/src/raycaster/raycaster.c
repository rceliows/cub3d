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

	fps_timer += r->frameTime;
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
	while (y < v->drawStart)
	{
		*(unsigned int *)pixel_ptr = r->ceiling_color;
		pixel_ptr += r->line_length;
		y++;
	}
	while (y <= v->drawEnd)
	{
		*(unsigned int *)pixel_ptr = wallColor;
		pixel_ptr += r->line_length;
		y++;
	}
	while (y < defScreenHeight)
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
	r->currentTime = tv.tv_sec + tv.tv_usec / 1000000.0;
	r->frameTime = r->currentTime - r->lastTime;
	r->lastTime = r->currentTime;
	if (r->frameTime > 0.1)
		r->frameTime = 0.1;
	r->moveSpeed = r->baseMovespeed * r->frameTime;
	r->rotSpeed = r->baseRotSpeed * r->frameTime;
}

static void	perform_dda(t_map *map, t_raycaster_var *v)
{
	int	hit;

	hit = 0;
	while (hit == 0)
	{
		if (v->sideDistX < v->sideDistY)
		{
			v->sideDistX += v->deltaDistX;
			v->mapX += v->stepX;
			v->side = 0;
		}
		else
		{
			v->sideDistY += v->deltaDistY;
			v->mapY += v->stepY;
			v->side = 1;
		}
		if (map->worldMap[v->mapX][v->mapY] > 0)
			hit = 1;
	}
}

static void	calculate_line(t_raycaster_var *v)
{
	if (v->side == 0)
		v->perpWallDist = (v->sideDistX - v->deltaDistX);
	else
		v->perpWallDist = (v->sideDistY - v->deltaDistY);
	v->lineHeight = (int)(defScreenHeight / v->perpWallDist);
	v->drawStart = -v->lineHeight / 2 + defScreenHeight / 2;
	if (v->drawStart < 0)
		v->drawStart = 0;
	v->drawEnd = v->lineHeight / 2 + defScreenHeight / 2;
	if (v->drawEnd >= defScreenHeight)
		v->drawEnd = defScreenHeight - 1;
}

static void	calculate_side_distance(t_raycaster *r, t_raycaster_var *v)
{
	if (v->rayDirX < 0)
	{
		v->stepX = -1;
		v->sideDistX = (r->posX - v->mapX) * v->deltaDistX;
	}
	else
	{
		v->stepX = 1;
		v->sideDistX = (v->mapX + 1.0 - r->posX) * v->deltaDistX;
	}
	if (v->rayDirY < 0)
	{
		v->stepY = -1;
		v->sideDistY = (r->posY - v->mapY) * v->deltaDistY;
	}
	else
	{
		v->stepY = 1;
		v->sideDistY = (v->mapY + 1.0 - r->posY) * v->deltaDistY;
	}
}

static void	calculate_ray(int x, t_raycaster *r, t_raycaster_var *v)
{
	v->cameraX = 2 * x / (double)defScreenWidth - 1;
	v->rayDirX = r->dirX + r->planeX * v->cameraX;
	v->rayDirY = r->dirY + r->planeY * v->cameraX;
	v->mapX = (int)(r->posX);
	v->mapY = (int)(r->posY);
	if (v->rayDirX == 0)
		v->deltaDistX = 1e30;
	else
		v->deltaDistX = fabs(1 / v->rayDirX);
	if (v->rayDirY == 0)
		v->deltaDistY = 1e30;
	else
		v->deltaDistY = fabs(1 / v->rayDirY);
}

static void	draw_image(int x, t_raycaster *r, t_raycaster_var *v)
{
	int		color;
	int		direction;

	if (v->side == 0) // vertical wall
	{
		if (v->stepX == -1)
			direction = 3; // West
		else
			direction = 4; // East
	}
	else // horizontal wall
	{
		if (v->stepY == -1)
			direction = 1; // North
		else
			direction = 2; // South
	}
	color = get_wall_color(direction);
	draw_column(r, x, v, color);
}

// void	*init_raycaster_var(t_raycaster_var *v)
// {
// 	v->cameraX = 0.0;
// 	v->rayDirX = 0.0;
// 	v->rayDirY = 0.0;
// 	v->mapX = 0;
// 	v->mapY = 0;
// 	v->sideDistX = 0.0;
// 	v->sideDistY = 0.0;
// 	v->deltaDistX = 0.0;
// 	v->deltaDistY = 0.0;
// 	v->perpWallDist = 0.0;
// 	v->stepX = 0;
// 	v->stepY = 0;
// 	v->side = 0;
// 	v->lineHeight = 0;
// 	v->drawStart = 0;
// 	v->drawEnd = 0;
// }

int	raycasting_function(t_raycaster *r, t_window *w,
			t_map *map)
{
	t_raycaster_var	*v;
	int				x;
	
	v = NULL;
	update_time_and_speed(r);
	process_movement(r, map);
	x = 0;
	while (x < defScreenWidth)
	{
		calculate_ray(x, r, v);
		calculate_side_distance(r, v);
		perform_dda(map, v);
		calculate_line(v);
		draw_image(x, r, v);
		x++;
	}
	mlx_put_image_to_window(w->mlx, w->win, w->img, 0, 0);
	display_fps(r, w);
	return (0);
}
