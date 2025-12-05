/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_drawing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:02:27 by rceliows          #+#    #+#             */
/*   Updated: 2025/11/05 16:02:28 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	get_pixel_color(char *img_data, t_sprite_var *vs,
		int line_length, int bits_per_pixel)
{
	char	*pixel_ptr;
	int		color;

	pixel_ptr = img_data + (vs->tex_y * line_length)
		+ (vs->tex_x * (bits_per_pixel / 8));
	color = *(unsigned int *)pixel_ptr;
	return (color);
}

static int	get_sprite_pixel(t_sprite_var *vs)
{
	char	*img_data;
	int		bits_per_pixel;
	int		line_length;
	int		endian;

	if (vs->tex_x < 0 || vs->tex_x >= vs->tex_width
		|| vs->tex_y < 0 || vs->tex_y >= vs->tex_height)
		return (0xFFFFFF);
	img_data = mlx_get_data_addr(vs->texture,
			&bits_per_pixel, &line_length, &endian);
	return (get_pixel_color(img_data, vs,
			line_length, bits_per_pixel));
}

static void	select_sprite_texture(t_cub3d *cub3d, t_sprite_data *sprites,
		int sprite_idx, t_sprite_var *vs)
{
	t_raycaster		*r;
	t_map			*map;
	static double	anim_timer = 0;
	static int		anim_frame = 0;

	r = cub3d->raycaster;
	map = cub3d->map;
	if (sprites->type[sprite_idx] == 4)
	{
		anim_timer += r->frame_time;
		if (anim_timer >= 1.0)
		{
			anim_frame = (anim_frame + 1) % 4;
			anim_timer = 0;
		}
		vs->texture = map->animated_sprite_frames[anim_frame];
		vs->tex_width = map->animated_sprite_width[anim_frame];
		vs->tex_height = map->animated_sprite_height[anim_frame];
	}
	else
	{
		vs->texture = map->texture[5];
		vs->tex_width = map->texture_width[5];
		vs->tex_height = map->texture_height[5];
	}
}

static void	draw_sprite_column(t_cub3d *cub3d, t_sprite_var *vs, int x)
{
	int	y;
	int	d;

	y = vs->draw_params[2];
	while (y < vs->draw_params[5])
	{
		d = (y - cub3d->raycaster->pitch) * 256
			- DEFSCREENHEIGHT * 128 + vs->draw_params[3] * 128;
		vs->tex_y = ((d * vs->tex_height) / vs->draw_params[3]) / 256;
		if (vs->tex_y >= 0 && vs->tex_y < vs->tex_height
			&& vs->tex_x >= 0 && vs->tex_x < vs->tex_width)
		{
			vs->color = get_sprite_pixel(vs);
			if ((vs->color & 0x00FFFFFF) != 0x00FFFFFF
				&& (vs->color & 0x00F0F0F0) != 0x00F0F0F0
				&& vs->color != 0xFFFFFF)
				set_pixel(x, y, vs->color, cub3d->raycaster);
		}
		y++;
	}
}

void	draw_sprite_stripe(t_cub3d *cub3d, t_sprite_data *sprites,
		int sprite_idx, t_sprite_var *vs)
{
	int	x;

	select_sprite_texture(cub3d, sprites, sprite_idx, vs);
	x = vs->draw_params[0];
	while (x < vs->draw_params[1])
	{
		vs->tex_x = (int)(256 * (x
					- (-vs->draw_params[3] / 2 + vs->draw_params[7]))
				* vs->tex_width / vs->draw_params[3]) / 256;
		if (vs->draw_params[6] > 0 && x > 0 && x < DEFSCREENWIDTH
			&& vs->draw_params[6] < vs->z_buffer[x])
			draw_sprite_column(cub3d, vs, x);
		x++;
	}
}
