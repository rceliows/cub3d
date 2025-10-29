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
	int color;

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

void	display_fps(t_raycaster *raycaster, t_window *window)
{
	static double	fps_timer = 0;
	static int		frame_count = 0;
	static double	current_fps = 0;
	char			fps_str[50];
	
	fps_timer += raycaster->frameTime;
	frame_count++;
	if (fps_timer >= 1.0)
	{
		current_fps = frame_count / fps_timer;
		frame_count = 0;
		fps_timer = 0;
	}
	sprintf(fps_str, "FPS: %.1f", current_fps);
	mlx_string_put(window->mlx, window->win, 10, 20, 0xFFFFFF, fps_str);
}

static void	draw_column(t_raycaster *raycaster, int x, int drawStart, int drawEnd, int wallColor)
{
	char			*pixel_ptr;
	int				pixel_size;
	int				y;

	pixel_size = raycaster->bits_per_pixel / 8;
	pixel_ptr = raycaster->img_data + x * pixel_size;
	y = 0;
	while (y < drawStart)
	{
		*(unsigned int *)pixel_ptr = raycaster->ceiling_color;
		pixel_ptr += raycaster->line_length;
		y++;
	}
	while (y <= drawEnd)
	{
		*(unsigned int *)pixel_ptr = wallColor;
		pixel_ptr += raycaster->line_length;
		y++;
	}
	while (y < raycaster->screenHeight)
	{
		*(unsigned int *)pixel_ptr = raycaster->floor_color;
		pixel_ptr += raycaster->line_length;
		y++;
	}
}

static void	init_raycaster_var(t_raycaster_var *var)
{
	var->tv.tv_sec = 0;
	var->tv.tv_usec = 0;
	var->currentTime = 0;
	var->cameraX = 0;
	var->rayDirX = 0;
	var->rayDirY = 0;
	var->mapX = 0;
	var->mapY = 0;
	var->sideDistX = 0;
	var->sideDistY = 0;
	var->deltaDistX = 0;
	var->deltaDistY = 0;
	var->perpWallDist = 0;
	var->stepX = 0;
	var->stepY = 0;
	var->side = 0;
	var->lineHeight = 0;
	var->drawStart = 0;
	var->drawEnd = 0;
}

int	raycasting_function(t_raycaster *raycaster, t_window *window, t_map *map)
{
	t_raycaster_var	var;
	int				x;
	int				hit;
	int				color;
	int				direction;

	init_raycaster_var(&var);
	
	// Update frame timing
	gettimeofday(&var.tv, NULL);
	var.currentTime = var.tv.tv_sec + var.tv.tv_usec / 1000000.0;
	raycaster->frameTime = var.currentTime - raycaster->lastTime;
	raycaster->lastTime = var.currentTime;
	if (raycaster->frameTime > 0.1)
		raycaster->frameTime = 0.1;
	
	// Update speeds based on frame time
	raycaster->moveSpeed = raycaster->baseMovespeed * raycaster->frameTime;
	raycaster->rotSpeed = raycaster->baseRotSpeed * raycaster->frameTime;
	
	// Process player movement
	process_movement(raycaster, map);
	
	// Raycasting loop
	x = 0;
	while (x < raycaster->screenWidth)
	{
		var.cameraX = 2 * x / (double)raycaster->screenWidth - 1;
		var.rayDirX = raycaster->dirX + raycaster->planeX * var.cameraX;
		var.rayDirY = raycaster->dirY + raycaster->planeY * var.cameraX;
		var.mapX = (int)(raycaster->posX);
		var.mapY = (int)(raycaster->posY);

		if (var.rayDirX == 0)
			var.deltaDistX = 1e30;
		else
			var.deltaDistX = fabs(1 / var.rayDirX);
		if (var.rayDirY == 0)
			var.deltaDistY = 1e30;
		else
			var.deltaDistY = fabs(1 / var.rayDirY);
		
		if (var.rayDirX < 0)
		{
			var.stepX = -1;
			var.sideDistX = (raycaster->posX - var.mapX) * var.deltaDistX;
		}
		else
		{
			var.stepX = 1;
			var.sideDistX = (var.mapX + 1.0 - raycaster->posX) * var.deltaDistX;
		}
		
		if (var.rayDirY < 0)
		{
			var.stepY = -1;
			var.sideDistY = (raycaster->posY - var.mapY) * var.deltaDistY;
		}
		else
		{
			var.stepY = 1;
			var.sideDistY = (var.mapY + 1.0 - raycaster->posY) * var.deltaDistY;
		}

		hit = 0;
		while (hit == 0)
		{
			if (var.sideDistX < var.sideDistY)
			{
				var.sideDistX += var.deltaDistX;
				var.mapX += var.stepX;
				var.side = 0;
			}
			else
			{
				var.sideDistY += var.deltaDistY;
				var.mapY += var.stepY;
				var.side = 1;
			}
			if (map->worldMap[var.mapX][var.mapY] > 0)
				hit = 1;
		}
		
		if (var.side == 0)
			var.perpWallDist = (var.sideDistX - var.deltaDistX);
		else
			var.perpWallDist = (var.sideDistY - var.deltaDistY);
		
		// Determine wall direction for color
		if (var.side == 0) // vertical wall
		{
			if (var.stepX == -1)
				direction = 3; // West
			else
				direction = 4; // East
		} 
		else // horizontal wall
		{
			if (var.stepY == -1)
				direction = 1; // North
			else
				direction = 2; // South
		}
		
		var.lineHeight = (int)(raycaster->screenHeight / var.perpWallDist);
		var.drawStart = -var.lineHeight / 2 + raycaster->screenHeight / 2;
		if (var.drawStart < 0)
			var.drawStart = 0;
		var.drawEnd = var.lineHeight / 2 + raycaster->screenHeight / 2;
		if (var.drawEnd >= raycaster->screenHeight)
			var.drawEnd = raycaster->screenHeight - 1;
		
		color = get_wall_color(direction);
		draw_column(raycaster, x, var.drawStart, var.drawEnd, color);
		x++;
	}
	
	mlx_put_image_to_window(window->mlx, window->win, 
		window->img, 0, 0);
	display_fps(raycaster, window);
	return (0);
}
