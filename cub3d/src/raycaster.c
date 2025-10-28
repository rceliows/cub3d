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

#define RED   "\x1b[31m"
#define BLUE  "\x1b[34m"
#define RESET "\x1b[0m"

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

void    verLine(t_raycaster *raycaster, int x, int y_start, int y_end, int color)
{
    int i = y_start;

    while(i <= y_end)
    {
        set_pixel(raycaster, x, i, color);
        i++;
    }
}

void    raycaster(t_raycaster *raycaster)
{
    // posX and posY start position (have to change it to the N/S/W/E one)
    raycaster.posX = 22;
    raycaster.posY = 12;

    // dirX and dirY direction we looking at at the start position
    double  dirX = raycaster->start_posX;
    double  dirY = raycaster->start_posY;

    // the camera plane
    double  planeX = 0;
    double  planeY = 0.66;

    // Getting the time
    struct timeval currentTV;
    struct timeval previousTV;
    double time = 0;                    // Time of current frame
    double oldTime = 0;                 // Time of previous frame  

    // Init for later
    gettimeofday(&previousTV, NULL);

    double  cameraX;                    // x coordinate of the camera
    double  rayDirX;
    double  rayDirY;
    
    // Box of the map we are in
    int mapX;
    int mapY;

    // Length of the ray from current position to the next x or y side
    double sideDistX;
    double sideDistY;

    // Length of the ray to reach from one x/y cube to the next
    double deltaDistX;
    double deltaDistY;

    // Distance from the screen plane to the wall drawn perpendicularly to avoid fisheye
    double perpWallDist;
    
    // Next cuadrant we'll step in either +1 or -1
    int stepX;
    int stepY;

    // Was a wall hit
    int wall_hit = 0;
    int side_hit;
    
    // Line calculation
    int lineHeight;
    int drawStart;
    int drawEnd;

    int color;

    // Time between frames calculation for modifiers 
    double frameTime;

    // Modifiers calculation
    double moveSpeed;
    double rotSpeed;

    int x = 0;
    while (1)
    {
        while( x < screenWidth)
        {
            cameraX = 2 * x / (double)screenWidth - 1;
            rayDirX = dirX + planeX * cameraX;
            rayDirY = dirY + planeY * cameraX;
            mapX = int(raycaster.posX);
            mapY = int(raycaster.posY);

            // Step and inicitial sideDist calculation
            if (rayDirX == 0)
                deltaDistX = 1e30;
            else
                deltaDistX = fabs(1.0 / rayDirX);
            if (rayDirY == 0)
                deltaDistY = 1e30;
            else
                deltaDistY = fabs(1.0 / rayDirY);
            if(rayDirX < 0)
            {
                stepX = -1;
                sideDistX = (raycaster.posX - mapX) * deltaDistX;
            }
            else
            {
                stepX = 1;
                sideDistX = (mapX + 1.0 - raycaster.posX) * deltaDistX;
            }
            if(rayDirY < 0)
            {
                stepY = -1;
                sideDistY = (raycaster.posY - mapY) * deltaDistY;
            }
            else
            {
                stepY = 1;
                sideDistY = (mapY + 1.0 - raycaster.posY) * deltaDistY;
            }

            // Perform DDA
            while(wall_hit == 0)
            {
                // Move accross the cuadrands until a wall is find
                if(sideDistX < sideDistY)
                {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side_hit = 0;
                }
                else
                {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side_hit = 1;
                }
                // Until we hit a wall and then we exit
                if(worldMap[mapX][mapY] > 0)
                    wall_hit = 1;
            }
            if(side == 0)
                perpWallDist = (sideDistX - deltaDistX);
            else
                perpWallDist = (sideDistY - deltaDistY);
            
            // Draw the line on the screen
            lineHeight = (int)(screenHeight / perpWallDist);
            drawStart = -lineHeight / 2 + h / 2;
            if(drawStart < 0)
                drawStart = 0;
            drawEnd = lineHeight / 2 + h / 2;
            if(drawEnd >= h)
                drawEnd = h - 1;
            if (side_hit = 0)
                color = RED;
            else
                color = BLUE;
            verLine(raycaster, x, drawStart, drawEnd, color);
            
            gettimeofday(&currentTV, NULL);
            oldTime = previousTV.tv_sec + previousTV.tv_usec * 1e-6;
            time = currentTV.tv_sec + currentTV.tv_usec * 1e-6;
        }
        x++;
    }
}