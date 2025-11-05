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
		if (map->world_map[v->map_x][v->map_y] == 1 ||
			map->world_map[v->map_x][v->map_y] == 2)
			hit = 1;
	}
}

static void	calculate_line(t_raycaster_var *v, t_raycaster *r)
{
	if (v->side == 0)
		v->perp_wall_dist = (v->side_dist_x - v->delta_dist_x);
	else
		v->perp_wall_dist = (v->side_dist_y - v->delta_dist_y);
	if (v->perp_wall_dist < 0.01)
		v->perp_wall_dist = 0.01;
	v->line_height = (int)(DEFSCREENHEIGHT / v->perp_wall_dist);
	if (v->line_height > DEFSCREENHEIGHT * 10)
		v->line_height = DEFSCREENHEIGHT * 10;
	v->draw_start = -v->line_height / 2 + DEFSCREENHEIGHT / 2 + r->pitch;
	if (v->draw_start < 0)
		v->draw_start = 0;
	v->draw_end = v->line_height / 2 + DEFSCREENHEIGHT / 2 + r->pitch;
	if (v->draw_end >= DEFSCREENHEIGHT)
		v->draw_end = DEFSCREENHEIGHT - 1 ;
	v->z_buffer[v->x] = v->perp_wall_dist;
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

static void	calculate_ray(t_raycaster *r, t_raycaster_var *v)
{
	v->camera_x = 2 * v->x / (double)DEFSCREENWIDTH - 1;
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

void	set_pixel(int x, int y, int color, t_raycaster *r)
{
	char	*pixel_ptr;

	if (x >= 0 && x < DEFSCREENWIDTH && y >= 0 && y < DEFSCREENHEIGHT)
	{
		pixel_ptr = r->img_data
			+ (y * r->line_length + x * (r->bits_per_pixel / 8));
		*(unsigned int *)pixel_ptr = color;
	}
}

int	get_texture_pixel(void *texture, t_raycaster_var *v,
		int tex_width, int tex_height)
{
	char	*img_data;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	char	*pixel_ptr;

	img_data = mlx_get_data_addr(texture,
			&bits_per_pixel, &line_length, &endian);
	v->actual_x = (v->tex_x * tex_width) / TEXTURESIZE;
	v->actual_y = (v->tex_y * tex_height) / TEXTURESIZE;
	if (v->actual_x < 0)
		v->actual_x = 0;
	if (v->actual_x >= tex_width)
		v->actual_x = tex_width - 1;
	if (v->actual_y < 0)
		v->actual_y = 0;
	if (v->actual_y >= tex_height)
		v->actual_y = tex_height - 1;
	pixel_ptr = img_data + (v->actual_y * line_length)
		+ (v->actual_x * (bits_per_pixel / 8));
	return (*(unsigned int *)pixel_ptr);
}

static void	draw_floor_ceiling(t_raycaster *r, t_raycaster_var *v)
{
	int	y;

	y = 0;
	while (y < v->draw_start)
	{
		if (y >= 0 && y < DEFSCREENHEIGHT)
			set_pixel(v->x, y, r->ceiling_color, r);
		y++;
	}
	y = v->draw_end;
	while (y < DEFSCREENHEIGHT)
	{
		if (y >= 0 && y < DEFSCREENHEIGHT)
			set_pixel(v->x, y, r->floor_color, r);
		y++;
	}
}

static void	calculate_texture_x(t_raycaster *r, t_raycaster_var *v)
{
	double	wall_x;

	if (v->side == 0)
		wall_x = r->pos_y + v->perp_wall_dist * v->ray_dir_y;
	else
		wall_x = r->pos_x + v->perp_wall_dist * v->ray_dir_x;
	wall_x -= floor(wall_x);
	v->tex_x = (int)(wall_x * (double)TEXTURESIZE);
	if (v->side == 0 && v->ray_dir_x < 0)
		v->tex_x = TEXTURESIZE - v->tex_x - 1;
	if (v->side == 1 && v->ray_dir_y > 0)
		v->tex_x = TEXTURESIZE - v->tex_x - 1;
	if (v->tex_x < 0)
		v->tex_x = 0;
	if (v->tex_x >= TEXTURESIZE)
		v->tex_x = TEXTURESIZE - 1;
}

static void	draw_walls(t_raycaster *r, t_raycaster_var *v, void *texture)
{
	int				y;
	int				d;
	unsigned int	color;

	calculate_texture_x(r, v);
	y = v->draw_start;
	while (y < v->draw_end)
	{
		if (y >= 0 && y < DEFSCREENHEIGHT)
		{
			d = (y - (DEFSCREENHEIGHT / 2 + r->pitch))
				* 256 + (v->line_height * 128);
			v->tex_y = ((d * TEXTURESIZE) / v->line_height) / 256;
			if (v->tex_y < 0)
				v->tex_y = 0;
			if (v->tex_y >= TEXTURESIZE)
				v->tex_y = TEXTURESIZE - 1;
			color = get_texture_pixel(texture, v, v->tex_width, v->tex_height);
			if (v->side == 1)
				color = (color >> 1) & 0x7F7F7F;
			set_pixel(v->x, y, color, r);
		}
		y++;
	}
}

static int	check_direction(t_raycaster_var *v)
{
	if (v->side == 0)
	{
		if (v->step_x == -1)
			return (2);
		else
			return (3);
	}
	else
	{
		if (v->step_y == -1)
			return (0);
		else
			return (1);
	}
}

static void	draw_image(t_raycaster *r, t_raycaster_var *v, t_map *map)
{
	int		direction;
	int		cell_type;

	draw_floor_ceiling(r, v);
	cell_type = map->world_map[v->map_x][v->map_y];
	if (cell_type == 2)
	{
		direction = 4;
		v->tex_width = map->texture_width[4];
		v->tex_height = map->texture_height[4];
		draw_walls(r, v, map->texture[4]);
		return ;
	}
	direction = check_direction(v);
	v->tex_width = map->texture_width[direction];
	v->tex_height = map->texture_height[direction];
	draw_walls(r, v, map->texture[direction]);
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

static void	display_minimap_row(t_raycaster *r,
			t_map *map, int i, int pixel_size)
{
	char	*pixel_ptr;
	int		crn_dst;
	int		j;

	j = 0;
	crn_dst = 10;
	while (j < MAPWIDTH)
	{
		pixel_ptr = r->img_data + (crn_dst + i * MINIMAPSIZE)
			* r->line_length + (crn_dst + j * MINIMAPSIZE) * pixel_size;
		if (i == (int)(r->pos_x) && j == (int)(r->pos_y))
			paint_sector(pixel_ptr, r->line_length, pixel_size, 0xFF0000);
		else if (map->world_map[i][j] == 1)
			paint_sector(pixel_ptr, r->line_length, pixel_size, 0xFFFFFF);
		else if (map->world_map[i][j] == 2)
			paint_sector(pixel_ptr, r->line_length, pixel_size, 0x8B4513);
		else if (map->world_map[i][j] == 3)
			paint_sector(pixel_ptr, r->line_length, pixel_size, 0x00FF00);
		else if (map->world_map[i][j] == 4)
			paint_sector(pixel_ptr, r->line_length, pixel_size, 0xFFFF00);
		j++;
	}
}

static void	display_minimap(t_raycaster *r, t_map *map)
{
	int	pixel_size;
	int	i;

	pixel_size = r->bits_per_pixel / 8;
	i = 0;
	while (i < MAPHEIGHT)
	{
		display_minimap_row(r, map, i, pixel_size);
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
	t_raycaster		*r;
	t_window		*w;
	t_map			*map;

	r = cub3d->raycaster;
	w = cub3d->window;
	map = cub3d->map;
	update_time_and_speed(r);
	update_doors(r, map);
	process_movement(r, map);
	v.x = 0;
	while (v.x < DEFSCREENWIDTH)
	{
		calculate_ray(r, &v);
		calculate_side_distance(r, &v);
		perform_dda(map, &v);
		calculate_line(&v, r);
		draw_image(r, &v, map);
		v.x++;
	}
	render_sprites(cub3d, v.z_buffer);
	mlx_put_image_to_window(w->mlx, w->win, w->img, 0, 0);
	bonus_elements(r, w, map);
	return (0);
}
