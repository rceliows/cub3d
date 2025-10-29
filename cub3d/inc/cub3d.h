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

# define MAPWIDTH 24
# define MAPHEIGHT 24 

# ifndef DEFSCREENWIDTH
#  define DEFSCREENWIDTH 1920
# endif

# ifndef DEFSCREENHEIGHT
#  define DEFSCREENHEIGHT 1080
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
	int		center_x;
	int		center_y;
}	t_window;

typedef struct s_map
{
	int		world_map[MAPWIDTH][MAPHEIGHT];
	int		north_texture;
	int		south_texture;
	int		east_texture;
	int		west_texture;
	int		ceiling_color;
	int		floor_color;
	double	start_position_x;
	double	start_position_y;
	int		start_direction;
}	t_map;

typedef struct s_raycaster
{
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	unsigned int	ceiling_color;
	unsigned int	floor_color;
	double			dir_x;
	double			dir_y;
	double			plane_x;
	double			plane_y;
	double			frame_time;
	double			last_time;
	double			screen_scale;
	double			base_move_speed;
	double			base_rot_speed;
	double			current_time;
	double			rot_speed;
	double			move_speed;
	double			pos_x;
	double			pos_y;
	char			*img_data;
	t_keys			keys;
}	t_raycaster;

typedef struct s_raycaster_var
{
	double			camera_x;
	double			ray_dir_x;
	double			ray_dir_y;
	int				map_x;
	int				map_y;
	double			side_dist_x;
	double			side_dist_y;
	double			delta_dist_x;
	double			delta_dist_y;
	double			perp_wall_dist;
	int				step_x;
	int				step_y;
	int				side;
	int				line_height;
	int				draw_start;
	int				draw_end;
}	t_raycaster_var;

typedef struct s_cub3d
{
	t_keys		*keys;
	t_window	*window;
	t_map		*map;
	t_raycaster	*raycaster;
}	t_cub3d;

/* Raycaster */
int			raycasting_function(t_cub3d *cub3d);

/* Key binds */
void		prep_hooks(t_cub3d *cub3d);
int			key_press(int keycode, t_cub3d *cub3d);
int			key_release(int keycode, t_cub3d *cub3d);
int			handle_close(t_cub3d *cub3d);
void		process_movement(t_raycaster *raycaster, t_map *map);
int			mouse_move(int x, int y, t_cub3d *cub3d);

/* Inits and cleanups */
t_keys		*init_keys(void);
t_map		*init_map(void);
t_window	*init_window(void);
t_raycaster	*init_raycaster(t_window *w, t_map *map, int direction);
void		cleanup_window(t_window *window);
void		cleanup_cub3d(t_cub3d *cub3d);

/* Utils */
void		error_exit(t_cub3d *raycaster);

#endif