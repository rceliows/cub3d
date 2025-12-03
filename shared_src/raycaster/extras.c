/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extras.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:39:13 by rceliows          #+#    #+#             */
/*   Updated: 2025/11/05 15:39:14 by rceliows         ###   ########.fr       */
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
