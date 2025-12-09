#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/inc/libft.h"
# include "../minilibx-linux/mlx.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define DEFSCREENWIDTH 1920
# define DEFSCREENHEIGHT 1080
# define TEXTURESIZE 64
# define MINIMAPSIZE 10

typedef struct s_data
{
	char	**map;
	char	*north_texture;
	char	*south_texture;
	char	*west_texture;
	char	*east_texture;
	int		floor[3];
	int		ceiling[3];
	int		have_floor;
	int		have_ceiling;
	char	player_dir;
	int		player_x;
	int		player_y;
}			t_data;

typedef struct s_color_ctx
{
	char	*path;
	int		*rgb;
	int		*hdr_count;
	int		*flag;
	char	type;
}	t_color_ctx;

typedef struct s_pmctx
{
	char	**map;
	int		map_count;
	int		map_cap;
	int		in_map;
	int		gap;
	int		hdr_count;
	int		fd;
	t_data	*game;
}			t_pmctx;

typedef struct s_keys
{
	int		w;
	int		a;
	int		s;
	int		d;
	int		left;
	int		right;
	int		minimap;
	int		fps;
	int		e;
	int		mouse_lock;
}			t_keys;

typedef struct s_door
{
	int		x;
	int		y;
	int		state;
	double	timer;
}			t_door;

typedef struct s_door_manager
{
	t_door	*doors;
	int		count;
	int		capacity;
}			t_door_manager;

typedef struct s_map
{
	int		**world_map;
	int		map_height;
	int		map_width;
	char	*n_xpm_path;
	char	*s_xpm_path;
	char	*e_xpm_path;
	char	*w_xpm_path;
	char	*door_path;
	char	*sprite_path;
	char	*animated_sprite_path[4];
	void	*texture[6];
	int		texture_width[6];
	int		texture_height[6];
	void	*animated_sprite_frames[4];
	int		animated_sprite_width[4];
	int		animated_sprite_height[4];
	int		ceiling_color;
	int		floor_color;
	double	start_position_x;
	double	start_position_y;
	char	start_direction;
}			t_map;

typedef struct s_window
{
	void	*mlx;
	void	*win;
	void	*img;
	int		center_x;
	int		center_y;
}			t_window;

typedef struct s_raycaster
{
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	char	*img_data;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		ceiling_color;
	int		floor_color;
	double	screen_scale;
	double	base_move_speed;
	double	base_rot_speed;
	double	last_time;
	double	current_time;
	double	frame_time;
	double	move_speed;
	double	rot_speed;
	int		pitch;
	t_keys	keys;
}			t_raycaster;

typedef struct s_raycaster_var
{
	int		x;
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	perp_wall_dist;
	int		step_x;
	int		step_y;
	int		side;
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		tex_x;
	int		tex_y;
	int		tex_width;
	int		tex_height;
	int		actual_x;
	int		actual_y;
	double	z_buffer[DEFSCREENWIDTH];
}			t_raycaster_var;

typedef struct s_sprite_data
{
	double	*pos_x;
	double	*pos_y;
	int		*type;
	int		*order;
	double	*distance;
	int		count;
}			t_sprite_data;

typedef struct s_sprite_var
{
	double	sprite_x;
	double	sprite_y;
	double	inv_det;
	double	transform_x;
	double	transform_y;
	int		sprite_screen_x;
	int		sprite_height;
	int		sprite_width;
	int		draw_start_y;
	int		draw_end_y;
	void	*texture;
	int		tex_width;
	int		tex_height;
	int		tex_x;
	int		tex_y;
	int		color;
	double	*z_buffer;
	int		draw_params[8];
}			t_sprite_var;

typedef struct s_cub3d
{
	t_keys		*keys;
	t_map		*map;
	t_window	*window;
	t_raycaster	*raycaster;
	t_data		*game;
}			t_cub3d;

// Initialization functions
t_keys			*init_keys(void);
t_map			*init_map(int height, int width);
t_window		*init_window(void);
t_raycaster		*init_raycaster(t_window *w, t_map *map, int direction);
int				init_doors(t_map *map);
int				collect_sprites(t_map *map,
					t_raycaster *r, t_sprite_data *sprites);

// Cleanup functions
void			cleanup_cub3d(t_cub3d *cub3d);
void			cleanup_map(t_map *map, t_window *w);
void			cleanup_window(t_window *w);
void			cleanup_doors(void);
void			cleanup_sprites(t_sprite_data *sprites);
void			error_exit(t_cub3d *cub3d);
void			clenup_game(t_data *game);
void			free_world_map(t_map *map);

// Parsing functions
int				verify_map(const char *path, t_data *game);
char			**parse_map(char *file, t_data *game);
int				check_n_set_header(char *line, t_data *g, int *hdr_count);
int				parse_rgb(char *line, int rgb[3]);
int				is_blank(const char *s);
void			rstrip_nl(char *s);
void			ft_free_split(char **arr);
int				map_check(char **map, int *player,
					int *map_size, int *iteration);
int				find_char_position(char **map, int *map_size, char c, int *pos);
char			**copy_map(char **map, int *map_size);
int				ensure_cap(char ***map, int *cap, int need);
int				push_map_line(char ***map, int *count, int *cap, char *line);
void			apply_parsed_to_map(t_map *m, t_data *g);
void			get_map_size(char **map, int *height, int *width);
char			*skip_ws(char *s);

// Validation functions
int				validate_map(t_data *game);
int				check_if_valid(const char *s);
int				has_cub_extension(const char *map);
int				handle_duplicate_color(int *flag, char type);
int				handle_empty_color(char *trimmed, char type);
int				handle_invalid_color(int rgb[3], char *trimmed, char type);
int				process_color_value(char *path, int rgb[3], char type);

// Image extraction
int				extract_images(t_map *map, t_window *w);
int				extract_images_bonus(t_map *map, t_window *w);

// Movement functions
void			apply_rotation(t_raycaster *r, double angle);
void			apply_forward(t_raycaster *r, t_map *map);
void			apply_backward(t_raycaster *r, t_map *map);
void			apply_strafe_right(t_raycaster *r, t_map *map);
void			apply_strafe_left(t_raycaster *r, t_map *map);
void			process_movement(t_raycaster *r, t_map *map);

// Key handling
int				key_press(int keycode, t_cub3d *cub3d);
int				key_release(int keycode, t_cub3d *cub3d);
int				handle_close(t_cub3d *cub3d);
int				mouse_move(int x, int y, t_cub3d *cub3d);
void			prep_hooks(t_cub3d *cub3d);

// Raycasting functions
int				raycasting_function(t_cub3d *cub3d);
void			calculate_ray(t_raycaster *r, t_raycaster_var *v);
void			calculate_side_distance(t_raycaster *r, t_raycaster_var *v);
void			calculate_line(t_raycaster_var *v, t_raycaster *r);
void			calculate_texture_x(t_raycaster *r, t_raycaster_var *v);
void			perform_dda(t_map *map, t_raycaster_var *v);
void			set_pixel(int x, int y, int color, t_raycaster *r);
int				get_texture_pixel(void *texture, t_raycaster_var *v,
					int tex_width, int tex_height);
void			update_time_and_speed(t_raycaster *r);

// Bonus elements
void			bonus_elements(t_raycaster *r, t_window *w, t_map *map);

// Door functions
t_door_manager	*get_door_manager(void);
int				is_door(t_map *map, int x, int y);
void			toggle_door(t_raycaster *r, t_map *map);
void			update_doors(t_raycaster *r, t_map *map);
int				get_door_state(int x, int y);

// Sprite functions
void			render_sprites(t_cub3d *cub3d, double *z_buffer);
void			draw_sprite_stripe(t_cub3d *cub3d, t_sprite_data *sprites,
					int sprite_idx, t_sprite_var *vs);

#endif
