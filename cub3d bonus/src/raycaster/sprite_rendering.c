/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_rendering.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 13:27:50 by rceliows          #+#    #+#             */
/*   Updated: 2025/11/04 13:27:51 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static void	sort_sprites(t_sprite_data *sprites)
{
	int		i;
	int		j;
	int		tmp_order;
	double	tmp_dist;

	i = 0;
	while (i < sprites->count - 1)
	{
		j = 0;
		while (j < sprites->count - i - 1)
		{
			if (sprites->distance[j] < sprites->distance[j + 1])
			{
				tmp_dist = sprites->distance[j];
				sprites->distance[j] = sprites->distance[j + 1];
				sprites->distance[j + 1] = tmp_dist;
				tmp_order = sprites->order[j];
				sprites->order[j] = sprites->order[j + 1];
				sprites->order[j + 1] = tmp_order;
			}
			j++;
		}
		i++;
	}
}

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

	if (vs->tex_x < 0 || vs->tex_x >= vs->tex_width || vs->tex_y < 0 || vs->tex_y >= vs->tex_height)
		return (0xFFFFFF);
	img_data = mlx_get_data_addr(vs->texture,
			&bits_per_pixel, &line_length, &endian);
	return (get_pixel_color(img_data, vs->tex_x, vs->tex_y,
			line_length, bits_per_pixel));
}


static void	select_sprite_texture(t_cub3d *cub3d, t_sprite_data *sprites,
		int sprite_idx, t_sprite_var *vs)
{
	t_raycaster	*r;
	t_map		*map;
	static double anim_timer = 0;
	static int anim_frame = 0;

	r = cub3d->raycaster;
	map = cub3d->map;
	if (sprites->type[sprite_idx] == 4)
	{
		anim_timer += r->frame_time;
		if (anim_timer >= 1.0)
		{
			anim_frame = (anim_frame + 1) % 3;
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
			vs->color = get_sprite_pixel(vs->texture, vs->tex_x,
					vs->tex_y, vs->tex_width, vs->tex_height);
			if ((vs->color & 0x00FFFFFF) != 0x00FFFFFF
				&& (vs->color & 0x00F0F0F0) != 0x00F0F0F0
				&& vs->color != 0xFFFFFF)
				set_pixel(x, y, vs->color, cub3d->raycaster);
		}
		y++;
	}
}

static void	draw_sprite_stripe(t_cub3d *cub3d, t_sprite_data *sprites,
		int sprite_idx, t_sprite_var *vs)
{
	int	x;

	select_sprite_texture(cub3d, sprites, sprite_idx, vs);
	x = vs->draw_params[0];
	while (x < vs->draw_params[1])
	{
		vs->tex_x = (int)(256 * (x - (-vs->draw_params[3] / 2 + vs->draw_params[7]))
				* vs->tex_width / vs->draw_params[3]) / 256;
		if (vs->draw_params[6] > 0 && x > 0 && x < DEFSCREENWIDTH
			&& vs->draw_params[6] < vs->z_buffer[x])
			draw_sprite_column(cub3d, vs, x);
		x++;
	}
}

static void	calculate_sprite_variables(t_raycaster *r, t_sprite_var *vs,
		t_sprite_data *s, int i)
{
	vs->sprite_x = s->pos_x[s->order[i]] - r->pos_x;
	vs->sprite_y = s->pos_y[s->order[i]] - r->pos_y;
	vs->inv_det = 1.0 / (r->plane_x * r->dir_y - r->dir_x * r->plane_y);
	vs->transform_x = vs->inv_det * (r->dir_y * vs->sprite_x
			- r->dir_x * vs->sprite_y);
	vs->transform_y = vs->inv_det * (-r->plane_y * vs->sprite_x
			+ r->plane_x * vs->sprite_y);
	vs->sprite_screen_x = (int)((DEFSCREENWIDTH / 2)
			* (1 + vs->transform_x / vs->transform_y));
	vs->sprite_height = abs((int)(DEFSCREENHEIGHT / vs->transform_y));
	vs->sprite_width = abs((int)(DEFSCREENHEIGHT / vs->transform_y));
	vs->draw_start_y = -vs->sprite_height / 2 + DEFSCREENHEIGHT / 2 + r->pitch;
	vs->draw_end_y = vs->sprite_height / 2 + DEFSCREENHEIGHT / 2 + r->pitch;
}

static void	calculate_sprite_params(t_sprite_var *vs)
{
	vs->draw_params[0] = -vs->sprite_width / 2 + vs->sprite_screen_x;
	vs->draw_params[1] = vs->sprite_width / 2 + vs->sprite_screen_x;
	vs->draw_params[2] = vs->draw_start_y;
	vs->draw_params[3] = vs->sprite_height;
	vs->draw_params[4] = -vs->sprite_width / 2 + vs->sprite_screen_x;
	vs->draw_params[5] = vs->draw_end_y;
	vs->draw_params[6] = vs->transform_y;
	vs->draw_params[7] = vs->sprite_screen_x;
	if (vs->draw_params[0] < 0)
		vs->draw_params[0] = 0;
	if (vs->draw_params[1] >= DEFSCREENWIDTH)
		vs->draw_params[1] = DEFSCREENWIDTH - 1;
	if (vs->draw_params[2] < 0)
		vs->draw_params[2] = 0;
	if (vs->draw_params[5] >= DEFSCREENHEIGHT)
		vs->draw_params[5] = DEFSCREENHEIGHT - 1;
}

void	render_sprites(t_cub3d *cub3d, double *z_buffer)
{
	t_sprite_data	sprites;
	t_sprite_var	vs;
	int				i;

	if (!collect_sprites(cub3d->map, cub3d->raycaster, &sprites)
		|| sprites.count == 0)
		return ;
	sort_sprites(&sprites);
	vs.z_buffer = z_buffer;
	i = 0;
	while (i < sprites.count)
	{
		calculate_sprite_variables(cub3d->raycaster, &vs, &sprites, i);
		calculate_sprite_params(&vs);
		draw_sprite_stripe(cub3d, &sprites, sprites.order[i],
			&vs);
		i++;
	}
	cleanup_sprites(&sprites);
}
