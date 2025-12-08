/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extras.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:51:04 by rceliows          #+#    #+#             */
/*   Updated: 2025/11/05 15:51:06 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static void	paint_sector(char *pixel_ptr, int line_length,
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
	while (j < map->map_width)
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
	while (i < map->map_height)
	{
		display_minimap_row(r, map, i, pixel_size);
		i++;
	}
}

static void	display_fps(t_raycaster *r, t_window *w)
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

void	bonus_elements(t_raycaster *r, t_window *w, t_map *map)
{
	if (r->keys.fps)
		display_fps(r, w);
	if (r->keys.minimap)
		display_minimap(r, map);
}
