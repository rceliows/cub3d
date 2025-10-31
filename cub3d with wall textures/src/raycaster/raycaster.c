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

int	get_wall_color(int direction)
{
	int	color;

	if (direction == 1)
		color = map->n_image;
	else if (direction == 2)
		color = map->s_image;
	else if (direction == 3)
		color = map->w_image;
	else if (direction == 4)
		color = map->e_image;
	else
		color = 0xFF00FF;
	return (color);
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

void	set_pixel(t_window *w, int x, int y, int color)
{
	char	*pixel_ptr;
    char	*img_data;
    int		bits_per_pixel;
    int		line_length;
    int		endian;

    img_data = mlx_get_data_addr(w->img, &bits_per_pixel, &line_length, &endian);
	if (x >= 0 && x < DEFSCREENWIDTH && y >= 0 && y < DEFSCREENHEIGHT)
	{
		pixel_ptr = w->img_data
			+ (y * w->line_length + x * (w->bits_per_pixel / 8));
		*(unsigned int *)pixel_ptr = color;
	}
}


/////////////////////////////////////////////////////////////////////////////////////

int get_color(int x, int y, int bits_per_pixel, char *img_data, int line_length)
{
    int pixel_size;
    char *pixel_ptr;
	int color;

	pixel_size = bits_per_pixel / 8;
	pixel_ptr = img_data + y * line_length + x * pixel_size;
    color = *(unsigned int *)pixel_ptr;
    return color;
}

int get_texture_pixel(void *texture, int x, int y)
{
    char *img_data;
    int bits_per_pixel;
    int line_length;
    int endian;

    img_data = mlx_get_data_addr(texture, &bits_per_pixel, &line_length, &endian);
    return (get_color(x, y, bits_per_pixel, img_data, line_length));
}

static void draw_walls(t_raycaster_var *v, int x, t_raycaster *r, void *texture, t_window *w)
{
	int			d;
    double		wall_x;
	int			tex_x;
	int			tex_y;
	int			y;
	unsigned int color;

    if(v->side == 0)
        wall_x = r->pos_Y + v->perp_wall_dist * v->ray_dir_y;
    else
        wall_x = r->posX + v->perp_wall_dist * v->ray_dir_x;
    wall_x -= floor(wall_x);
	tex_x = (int)(wall_x * (double)TEXTURESIZE);
    if (v->side == 0 && r->rayDirX > 0)
		tex_x = TEXTURESIZE - tex_x - 1;
    if (v->side == 1 && r->rayDirY < 0)
		tex_x = TEXTURESIZE - tex_x - 1;
	y = v->draw_start
    while (y < v->draw_end)
    {
        d = y * 256 - DEFSCREENHEIGHT * 128 + r->lineHeight * 128;
        tex_y = ((d * TEXTURESIZE) / r->lineHeight) / 256;
        color = get_texture_pixel(texture, tex_x, tex_y);
        if (r->side == 1)
            color = (color >> 1) & 0x7F7F7F;
        set_pixel(w, x, y, color);
		y++;
    }
}


static void	draw_image(int x, t_raycaster *r, t_raycaster_var *v, t_map *map, t_window *w)
{
	int		direction;

	if (v->side == 0)
	{
		if (v->step_x == -1)
			direction = 2;
		else
			direction = 3;
	}
	else
	{
		if (v->step_y == -1)
			direction = 0;
		else
			direction = 1;
	}
	draw_walls(r, x, v, map->texture[direction], w);
}

void	paint_sector(char *pixel_ptr, int line_length,
				int pixel_size, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < MINIMAPSIZE)
	{
		j = 0;
		while (j < MINIMAPSIZE)
		{
			*(unsigned int *)(pixel_ptr + j * pixel_size) = color;
			j++;
		}
		pixel_ptr += line_length;
		i++;
	}
}

static void	display_minimap(t_raycaster *r, t_map *map)
{
	char	*pixel_ptr;
	int		pixel_size;
	int		crn_dst;
	int		i;
	int		j;

	pixel_size = r->bits_per_pixel / 8;
	crn_dst = 10;
	i = 0;
	while (i < MAPHEIGHT)
	{
		j = 0;
		while (j < MAPWIDTH)
		{
			pixel_ptr = r->img_data + (crn_dst + i * MINIMAPSIZE)
				* r->line_length + (crn_dst + j * MINIMAPSIZE) * pixel_size;
			if (i == (int)(r->pos_x) && j == (int)(r->pos_y))
				paint_sector(pixel_ptr, r->line_length, pixel_size, 0xFF0000);
			if (map->world_map[i][j] == 1)
				paint_sector(pixel_ptr, r->line_length, pixel_size, 0xFFFFFF);
			j++;
		}
		i++;
	}
}

void	display_fps(t_raycaster *r, t_window *w)
{
	static double	fps_timer = 0;
	static int		frame_count = 0;
	static double	current_fps = 0;
	char			*fps_str;

	fps_timer += r->frame_time;
	frame_count++;
	if (fps_timer >= 1.0)
	{
		current_fps = frame_count / fps_timer;
		frame_count = 0;
		fps_timer = 0;
	}
	fps_str = ft_itoa((int)current_fps);
	mlx_string_put(w->mlx, w->win, 10,
		(DEFSCREENHEIGHT - 8), 0xFFFFFF, fps_str);
	free(fps_str);
}

static void	bonus_elements(t_raycaster *r, t_window *w, t_map *map)
{
	if (r->keys.fps)
		display_fps(r, w);
	if (r->keys.minimap)
		display_minimap(r, map);
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
	r->keys = *cub3d->keys;
	update_time_and_speed(r);
	process_movement(r, map);
	x = 0;
	while (x < DEFSCREENWIDTH)
	{
		calculate_ray(x, r, &v);
		calculate_side_distance(r, &v);
		perform_dda(map, &v);
		calculate_line(&v);
		draw_image(x, r, &v, map, w);
		x++;
	}
	mlx_put_image_to_window(w->mlx, w->win, w->img, 0, 0);
	bonus_elements(r, w, map);
	return (0);
}
