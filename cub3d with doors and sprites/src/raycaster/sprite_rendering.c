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

static void	collect_sprites(t_map *map, t_raycaster *r, t_sprite_data *sprites)
{
	int	i;
	int	j;
	int	idx;

	sprites->count = 0;
	i = 0;
	while (i < MAPHEIGHT)
	{
		j = 0;
		while (j < MAPWIDTH)
		{
			if (map->world_map[i][j] == 3 || map->world_map[i][j] == 4)
				sprites->count++;
			j++;
		}
		i++;
	}
	sprites->pos_x = malloc(sizeof(double) * sprites->count);
	sprites->pos_y = malloc(sizeof(double) * sprites->count);
	sprites->type = malloc(sizeof(int) * sprites->count);
	sprites->order = malloc(sizeof(int) * sprites->count);
	sprites->distance = malloc(sizeof(double) * sprites->count);
	idx = 0;
	i = 0;
	while (i < MAPHEIGHT)
	{
		j = 0;
		while (j < MAPWIDTH)
		{
			if (map->world_map[i][j] == 3 || map->world_map[i][j] == 4)
			{
				sprites->pos_x[idx] = i + 0.5;
				sprites->pos_y[idx] = j + 0.5;
				sprites->type[idx] = map->world_map[i][j];
				sprites->order[idx] = idx;
				sprites->distance[idx] = ((r->pos_x - sprites->pos_x[idx])
					* (r->pos_x - sprites->pos_x[idx])
					+ (r->pos_y - sprites->pos_y[idx])
					* (r->pos_y - sprites->pos_y[idx]));
				idx++;
			}
			j++;
		}
		i++;
	}
}

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

static int	get_sprite_pixel(void *texture, int tex_x, int tex_y,
		int tex_width, int tex_height)
{
	char	*img_data;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	char	*pixel_ptr;
	int		color;

	if (tex_x < 0 || tex_x >= tex_width || tex_y < 0 || tex_y >= tex_height)
		return (0xFFFFFF);
	img_data = mlx_get_data_addr(texture,
			&bits_per_pixel, &line_length, &endian);
	pixel_ptr = img_data + (tex_y * line_length)
		+ (tex_x * (bits_per_pixel / 8));
	color = *(unsigned int *)pixel_ptr;
	return (color);
}

static void	draw_sprite_stripe(t_cub3d *cub3d, t_sprite_data *sprites,
		int sprite_idx, double *z_buffer, int *draw_params)
{
	int		x;
	int		y;
	int		tex_x;
	int		tex_y;
	int		color;
	void	*texture;
	int		tex_width;
	int		tex_height;
	t_raycaster	*r;
	t_map	*map;
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
		texture = map->animated_sprite_frames[anim_frame];
		tex_width = map->animated_sprite_width[anim_frame];
		tex_height = map->animated_sprite_height[anim_frame];
	}
	else
	{
		texture = map->texture[5];
		tex_width = map->texture_width[5];
		tex_height = map->texture_height[5];
	}
	x = draw_params[0];
	while (x < draw_params[1])
	{
		tex_x = (int)(256 * (x - (-draw_params[3] / 2 + draw_params[7])) * tex_width / draw_params[3]) / 256;
		if (draw_params[6] > 0 && x > 0 && x < DEFSCREENWIDTH
			&& draw_params[6] < z_buffer[x])
		{
			y = draw_params[2];
			while (y < draw_params[5])
			{
				int d = (y - r->pitch) * 256 - DEFSCREENHEIGHT * 128 + draw_params[3] * 128;
				tex_y = ((d * tex_height) / draw_params[3]) / 256;
				
				if (tex_y >= 0 && tex_y < tex_height && tex_x >= 0 && tex_x < tex_width)
				{
					color = get_sprite_pixel(texture, tex_x, tex_y,
							tex_width, tex_height);
					if ((color & 0x00FFFFFF) != 0x00FFFFFF
						&& (color & 0x00F0F0F0) != 0x00F0F0F0
						&& color != 0xFFFFFF)
						set_pixel(x, y, color, r);
				}
				y++;
			}
		}
		x++;
	}
}

static void	calculate_sprite_params(t_raycaster *r, t_sprite_data *sprites,
		int i, int *params)
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

	sprite_x = sprites->pos_x[sprites->order[i]] - r->pos_x;
	sprite_y = sprites->pos_y[sprites->order[i]] - r->pos_y;
	inv_det = 1.0 / (r->plane_x * r->dir_y - r->dir_x * r->plane_y);
	transform_x = inv_det * (r->dir_y * sprite_x - r->dir_x * sprite_y);
	transform_y = inv_det * (-r->plane_y * sprite_x + r->plane_x * sprite_y);
	sprite_screen_x = (int)((DEFSCREENWIDTH / 2)
			* (1 + transform_x / transform_y));
	sprite_height = abs((int)(DEFSCREENHEIGHT / transform_y));
	sprite_width = abs((int)(DEFSCREENHEIGHT / transform_y));
	draw_start_y = -sprite_height / 2 + DEFSCREENHEIGHT / 2 + r->pitch;
	draw_end_y = sprite_height / 2 + DEFSCREENHEIGHT / 2 + r->pitch;
	params[0] = -sprite_width / 2 + sprite_screen_x;
	params[1] = sprite_width / 2 + sprite_screen_x;
	params[2] = draw_start_y;
	params[3] = sprite_height;
	params[4] = -sprite_width / 2 + sprite_screen_x;
	params[5] = draw_end_y;
	params[6] = transform_y;
	params[7] = sprite_screen_x;
	if (params[0] < 0)
		params[0] = 0;
	if (params[1] >= DEFSCREENWIDTH)
		params[1] = DEFSCREENWIDTH - 1;
	if (params[2] < 0)
		params[2] = 0;
	if (params[5] >= DEFSCREENHEIGHT)
		params[5] = DEFSCREENHEIGHT - 1;
}

void	render_sprites(t_cub3d *cub3d, double *z_buffer)
{
	t_sprite_data	sprites;
	int				i;
	int				draw_params[8];

	collect_sprites(cub3d->map, cub3d->raycaster, &sprites);
	if (sprites.count == 0)
		return ;
	sort_sprites(&sprites);
	i = 0;
	while (i < sprites.count)
	{
		calculate_sprite_params(cub3d->raycaster, &sprites, i, draw_params);
		draw_sprite_stripe(cub3d, &sprites, sprites.order[i], z_buffer, draw_params);
		i++;
	}
	free(sprites.pos_x);
	free(sprites.pos_y);
	free(sprites.type);
	free(sprites.order);
	free(sprites.distance);
}
