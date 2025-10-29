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

# ifndef defScreenWidth
#  define defScreenWidth 1920
# endif

# ifndef defScreenHeight
#  define defScreenHeight 1080
# endif

# include "../minilibx-linux/mlx.h"
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>
# include <math.h>
# include <string.h>

/* Forward declaration of structures */
typedef struct s_pipex			t_pipex;

typedef struct s_keys
{
	int	w;
	int	a;
	int	s;
	int	d;
	int	left;
	int	right;
}	t_keys;

typedef struct s_window
{
	void	*mlx;
	void	*win;
	void	*img;
//	double	imag;
	int		center_x;
	int		center_y;
}	t_window;

typedef struct s_map
{
	int	worldMap[mapWidth][mapHeight];
	int	NorthTexture;
	int	SouthTexture;
	int	EastTexture;
	int	WestTexture;;
	int	CeilingColor;
	int FloorColor;
	double	start_positionX;
	double	start_positionY;
	int	start_direction;
}	t_map;

typedef struct s_raycaster
{
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		oldTime;
	unsigned int	ceiling_color;
	unsigned int	floor_color;
	double	dirX;
	double	dirY;
	double	planeX;
	double	planeY;
	double	frameTime;
	double	lastTime;
	double	screenScale;
	double	baseMovespeed;
	double	baseRotSpeed;
	double	rotSpeed;
	double	moveSpeed;
	double	posX;
	double	posY;
	char	*img_data;
	t_keys	keys;
}	t_raycaster;

typedef struct s_raycaster_var
{
	// Timing
	double			currentTime;
	
	// Ray calculation
	double			cameraX;
	double			rayDirX;
	double			rayDirY;
	
	// Map position
	int				mapX;
	int				mapY;
	
	// Distance calculation
	double			sideDistX;
	double			sideDistY;
	double			deltaDistX;
	double			deltaDistY;
	double			perpWallDist;
	
	// Step direction
	int				stepX;
	int				stepY;
	
	// Wall detection
	int				side;
	
	// Drawing
	int				lineHeight;
	int				drawStart;
	int				drawEnd;
}	t_raycaster_var;

typedef struct s_cub3d
{
	t_keys	*keys;
	t_window	*window;
	t_map	*map;
	t_raycaster	*raycaster;
	t_raycaster_var *var;
}	t_cub3d;

/* Raycaster */
int		raycasting_function(t_raycaster *raycaster, t_window *window, t_map *map, t_raycaster_var *var);

/* Key binds */
void	prep_hooks(t_raycaster *raycaster, t_window *window);
int		key_press(int keycode, t_raycaster *raycaster, t_cub3d *cub3d);
int		key_release(int keycode, t_raycaster *raycaster);
int		handle_close(t_cub3d *cub3d);
void	process_movement(t_raycaster *raycaster, t_map *map);
int		mouse_move(int x, int y, t_raycaster *raycaster, t_window *window);

/* Inits and cleanups */
t_keys	*init_keys(void);
t_map	*init_map(void);
t_window	*init_window(void);
t_raycaster	*init_raycaster(t_window *w, int direction);
t_raycaster_var	*init_raycaster_var(void);
void	cleanup_window(t_window *window);
// void	cleanup_raycaster(t_raycaster *raycaster);
void	cleanup_cub3d(t_cub3d *cub3d);

/* Utils */
void	error_exit(t_cub3d *raycaster);

#endif
