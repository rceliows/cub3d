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

# define MAPHEIGHT 24
# define MAPWIDTH 24

# define MINIMAPSIZE 6

# define TEXTURESIZE 24

# ifndef DEFSCREENWIDTH
#  define DEFSCREENWIDTH 1920
# endif

# ifndef DEFSCREENHEIGHT
#  define DEFSCREENHEIGHT 1080
# endif

# include "../libft/inc/ft_printf.h"
# include "../libft/inc/libft.h"
# include "../minilibx-linux/mlx.h"
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>

/* Forward declaration of structures */
typedef struct s_pipex	t_pipex;

typedef struct s_keys
{
	int					w;
	int					a;
	int					s;
	int					d;
	int					left;
	int					right;
	int					minimap;
	int					fps;
}						t_keys;

typedef struct s_window
{
	void				*mlx;
	void				*win;
	void				*img;
	int					center_x;
	int					center_y;
}						t_window;

typedef struct s_map
{
	int					**world_map;
	int					map_height;
	int					map_width;
	// int					world_map[MAPHEIGHT][MAPWIDTH];
	char				*n_xpm_path;
	char				*s_xpm_path;
	char				*e_xpm_path;
	char				*w_xpm_path;
	void				*texture[4];
	int					texture_width[4];
	int					texture_height[4];
	void				*n_image;
	void				*s_image;
	void				*e_image;
	void				*w_image;
	int					ceiling_color;
	int					floor_color;
	double				start_position_x;
	double				start_position_y;
	int					start_direction;
}						t_map;

typedef struct s_raycaster
{
	int					bits_per_pixel;
	int					line_length;
	int					endian;
	int					pitch;
	unsigned int		ceiling_color;
	unsigned int		floor_color;
	double				dir_x;
	double				dir_y;
	double				plane_x;
	double				plane_y;
	double				frame_time;
	double				last_time;
	double				screen_scale;
	double				base_move_speed;
	double				base_rot_speed;
	double				current_time;
	double				rot_speed;
	double				move_speed;
	double				pos_x;
	double				pos_y;
	char				*img_data;
	t_keys				keys;
}						t_raycaster;

typedef struct s_raycaster_var
{
	double				camera_x;
	double				ray_dir_x;
	double				ray_dir_y;
	int					map_x;
	int					map_y;
	double				side_dist_x;
	double				side_dist_y;
	double				delta_dist_x;
	double				delta_dist_y;
	double				perp_wall_dist;
	int					tex_width;
	int					tex_height;
	int					tex_x;
	int					tex_y;
	int					actual_x;
	int					actual_y;
	int					step_x;
	int					step_y;
	int					side;
	int					line_height;
	int					draw_start;
	int					draw_end;
}						t_raycaster_var;

typedef struct s_data
{
	char				**map;
	char				*north_texture;
	char				*south_texture;
	char				*west_texture;
	char				*east_texture;
	int					floor[3];
	int					ceiling[3];
	int					have_floor;
	int					have_ceiling;
	int					player_x;
	int					player_y;
	char				player_dir;
}						t_data;

typedef struct s_cub3d
{
	t_data				*game;
	t_keys				*keys;
	t_window			*window;
	t_map				*map;
	t_raycaster			*raycaster;
}						t_cub3d;

typedef struct s_pmctx
{
	char				**map;
	int					map_count;
	int					map_cap;
	int					in_map;
	int					gap;
	int					hdr_count;
	int					fd;
	t_data				*game;
}						t_pmctx;

/* Raycaster */
int						raycasting_function(t_cub3d *cub3d);
void					perform_dda(t_map *map, t_raycaster_var *v);
void					update_time_and_speed(t_raycaster *r);

/* Raycaster calculations */
void					calculate_line(t_raycaster_var *v, t_raycaster *r);
void					calculate_side_distance(t_raycaster *r,
							t_raycaster_var *v);
void					calculate_ray(int x, t_raycaster *r,
							t_raycaster_var *v);
void					calculate_texture_x(t_raycaster *r, t_raycaster_var *v);

/* Key and mouse actions */
void					prep_hooks(t_cub3d *cub3d);
int						key_press(int keycode, t_cub3d *cub3d);
int						key_press_bonus(int keycode, t_raycaster *r);
int						key_press_movement(int keycode, t_raycaster *r);
int						key_release(int keycode, t_cub3d *cub3d);
int						key_release_movement(int keycode, t_raycaster *r);
int						handle_close(t_cub3d *cub3d);
void					process_movement(t_raycaster *raycaster, t_map *map);
int						mouse_move(int x, int y, t_cub3d *cub3d);

/* Inits and cleanups */
t_keys					*init_keys(void);
t_map					*init_map(int height, int width);
// t_map					*init_map(t_window *w);
t_window				*init_window(void);
t_raycaster				*init_raycaster(t_window *w, t_map *map, int direction);
void					cleanup_map(t_map *map, t_window *w);
void					cleanup_window(t_window *window);
void					cleanup_cub3d(t_cub3d *cub3d);
void					clenup_game(t_data *game);

/* Movement */
void					apply_rotation(t_raycaster *r, double angle);
void					apply_forward(t_raycaster *r, t_map *map);
void					apply_backward(t_raycaster *r, t_map *map);
void					apply_strafe_right(t_raycaster *r, t_map *map);
void					apply_strafe_left(t_raycaster *r, t_map *map);

/* Utils */
void					error_exit(t_cub3d *raycaster);
void					set_pixel(int x, int y, int color, t_raycaster *r);

/* Bonus */
void					bonus_elements(t_raycaster *r, t_window *w, t_map *map);

// parsing
// t_data					*verify_map(const char *path);
int						verify_map(const char *path, t_data *game);
void					rstrip_nl(char *s);
int						validate_map(t_data *game);
void					ft_free_split(char **arr);
int						map_check(char **map, int *player, int *map_size,
							int *iteration);
int						find_char_position(char **map, int *map_size, char c,
							int *position);
void					apply_parsed_to_map(t_map *map, t_data *game);

int						extract_images(t_map *map, t_window *w);
void					get_map_size(char **map, int *height, int *width);

// cleanup
void					cleanup_cub3d(t_cub3d *cub3d);
void					free_world_map(t_map *map);
// copy_map
char					**copy_map(char **map, int *map_size);
// header setter
int						check_n_set_header(char *line, t_data *g,
							int *hdr_count);

int						parse_rgb(char *line, int rgb[3]);

// parse_map_helpers
int						check_if_valid(const char *s);
int						is_blank(const char *s);
int						ft_isnumber(char *str);
int						has_cub_extension(const char *map);

// verify files
int						has_xpm_extension(const char *path);
int						is_valid_texture_path(const char *raw);
int						validate_textures(t_data *game);
int						verify_map(const char *path, t_data *game);

// big ones
char					**parse_map(char *file, t_data *game);
int						push_map_line(char ***map, int *map_count, int *map_cap,
							char *line);
int						ensure_cap(char ***map, int *cap, int need);
void					copy_old_rows(char ***map, char **new_map);

#endif
