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

static void	draw_floor_ceiling(int x, t_raycaster *r, t_raycaster_var *v)
{
	int	y;

	y = 0;
	while (y < v->draw_start)
	{
		if (y >= 0 && y < DEFSCREENHEIGHT)
			set_pixel(x, y, r->ceiling_color, r);
		y++;
	}
	y = v->draw_end;
	while (y < DEFSCREENHEIGHT)
	{
		if (y >= 0 && y < DEFSCREENHEIGHT)
			set_pixel(x, y, r->floor_color, r);
		y++;
	}
}

static void	draw_walls(t_raycaster *r, int x, t_raycaster_var *v, void *texture)
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
			set_pixel(x, y, color, r);
		}
		y++;
	}
}

static void	draw_image(int x, t_raycaster *r, t_raycaster_var *v, t_map *map)
{
	int		direction;

	draw_floor_ceiling(x, r, v);
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
	mlx_get_data_addr(map->texture[direction],
		&v->tex_width, &v->tex_width, &v->tex_width);
	v->tex_width = map->texture_width[direction];
	v->tex_height = map->texture_height[direction];
	draw_walls(r, x, v, map->texture[direction]);
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
	update_time_and_speed(r);
	process_movement(r, map);
	x = 0;
	while (x < DEFSCREENWIDTH)
	{
		calculate_ray(x, r, &v);
		calculate_side_distance(r, &v);
		perform_dda(map, &v);
		calculate_line(&v, r);
		draw_image(x, r, &v, map);
		x++;
	}
	mlx_put_image_to_window(w->mlx, w->win, w->img, 0, 0);
	bonus_elements(r, w, map);
	return (0);
}
