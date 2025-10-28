/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 15:09:36 by rceliows          #+#    #+#             */
/*   Updated: 2025/10/28 15:09:37 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define mapWidth 24
# define mapHeight 24
# define screenWidth 640
# define screenHeight 480

# include "../minilibx-linux/mlx.h"
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>
# include <math.h>
#include <string.h>

typedef struct s_raycaster
{
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		oldTime;
	double	start_posX;
	double	start_posY;
	double	dirX;
	double	dirY;
	double	planeX;
	double	planeY;
	double	rotSpeed;
	double	moveSpeed;
	double	posX;
	double	posY;
	void	*mlx;
	void	*win;
	void	*img;
	double	imag;
	char	*img_data;
}	t_raycaster;

// Global world map
extern int worldMap[mapWidth][mapHeight];

// Raycaster
int		raycasting_function(t_raycaster *raycaster);

// Key binds
void	prep_hooks(t_raycaster *raycaster);
int		key_dispatcher(int keycode, t_raycaster *raycaster);
int		handle_close(t_raycaster *raycaster);

// Utils
void	init_raycaster(t_raycaster *raycaster);
void	cleanup_raycaster(t_raycaster *raycaster);
void	error_exit(t_raycaster *raycaster);

#endif