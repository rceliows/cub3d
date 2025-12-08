/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:51:12 by rceliows          #+#    #+#             */
/*   Updated: 2025/11/05 15:51:14 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	update_time_and_speed(t_raycaster *r)
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

void	perform_dda(t_map *map, t_raycaster_var *v)
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
