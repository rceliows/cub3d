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

void	set_pixel(t_raycaster *raycaster, int x, int y, int color)
{
	char	*pixel_ptr;

	if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight)
	{
		pixel_ptr = raycaster->img_data
			+ (y * raycaster->line_length + x * (raycaster->bits_per_pixel / 8));
		*(unsigned int *)pixel_ptr = color;
	}
}

void	verLine(t_raycaster *raycaster, int x, int y_start, int y_end, int color)
{
	int i;

	i = y_start;
	while(i <= y_end)
	{
		set_pixel(raycaster, x, i, color);
		i++;
	}
}

int	get_wall_color(int map_value, int side)
{
	int color;

	if (map_value == 1)
		color = RED;
	else if (map_value == 2)
		color = GREEN;
	else if (map_value == 3)
		color = BLUE;
	else if (map_value == 4)
		color = WHITE;
	else
		color = YELLOW;
	if (side == 1)
		color = color / 2;
	return (color);
}

int	raycasting_function(t_raycaster *raycaster)
{
	struct timeval currentTV;
	double time;
	double frameTime;
	double cameraX;
	double rayDirX;
	double rayDirY;

    // Box of the map we are in
	int mapX;
	int mapY;

	// Length of the ray from current position to the next x or y side
	double sideDistX;
	double sideDistY;

    // Length of the ray to reach from one x/y cube to the next
	double deltaDistX;
	double deltaDistY;

	double perpWallDist;

    // Next cuadrant we'll step in either +1 or -1
	int stepX;
	int stepY;

    // Wall hit detection
	int hit;
	int side;

    // Wall line calculation
	int lineHeight;
	int drawStart;
	int drawEnd;

	int color;
	int x;

	memset(raycaster->img_data, 0, screenHeight * raycaster->line_length);
	x = 0;
	while(x < screenWidth)
	{
		cameraX = 2 * x / (double)screenWidth - 1;
		rayDirX = raycaster->dirX + raycaster->planeX * cameraX;
		rayDirY = raycaster->dirY + raycaster->planeY * cameraX;
		mapX = (int)(raycaster->posX);
		mapY = (int)(raycaster->posY);

		if (rayDirX == 0)
			deltaDistX = 1e30;
		else
			deltaDistX = fabs(1 / rayDirX);
		if (rayDirY == 0)
			deltaDistY = 1e30;
		else
			deltaDistY = fabs(1 / rayDirY);
		if(rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (raycaster->posX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - raycaster->posX) * deltaDistX;
		}
		if(rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (raycaster->posY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - raycaster->posY) * deltaDistY;
		}

		hit = 0;
		while(hit == 0)
		{
			if(sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			if(worldMap[mapX][mapY] > 0)
				hit = 1;
		}
		if(side == 0)
			perpWallDist = (sideDistX - deltaDistX);
		else
			perpWallDist = (sideDistY - deltaDistY);
		
		lineHeight = (int)(screenHeight / perpWallDist);
		drawStart = -lineHeight / 2 + screenHeight / 2;
		if(drawStart < 0)
			drawStart = 0;
		drawEnd = lineHeight / 2 + screenHeight / 2;
		if(drawEnd >= screenHeight)
			drawEnd = screenHeight - 1;
		color = get_wall_color(worldMap[mapX][mapY], side);
		verLine(raycaster, x, drawStart, drawEnd, color);
		x++;
	}
	gettimeofday(&currentTV, NULL);
	time = currentTV.tv_sec + currentTV.tv_usec * 1e-6;
	frameTime = time - raycaster->oldTime;
	raycaster->oldTime = time;
	raycaster->moveSpeed = frameTime * 5.0;
	raycaster->rotSpeed = frameTime * 3.0;
	mlx_put_image_to_window(raycaster->mlx, raycaster->win, raycaster->img, 0, 0);
	return (0);
}