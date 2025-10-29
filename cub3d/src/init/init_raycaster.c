/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_raycaster.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 16:38:37 by rceliows          #+#    #+#             */
/*   Updated: 2025/10/29 16:38:38 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	setup_direction(t_raycaster *r, int direction)
{
	if (direction == 'N')
	{
		r->dirX = 0;
		r->dirY = -1;
	}
	else if (direction == 'S')
	{
		r->dirX = 0;
		r->dirY = 1;
	}
	else if (direction == 'E')
	{
		r->dirX = 1;
		r->dirY = 0;
	}
	else if (direction == 'W')
	{
		r->dirX = -1;
		r->dirY = 0;
	}
}

t_raycaster	*init_raycaster(t_window *w, int direction)
{
	t_raycaster *r;
	struct timeval	tv;

	r = malloc(sizeof(t_raycaster));
	if (!r)
		return (NULL);
	r->img_data = mlx_get_data_addr(w->img,
			&r->bits_per_pixel, &r->line_length, &r->endian);
	r->posX = 22.0;
	r->posY = 12.0;
	setup_direction(r, direction);
	r->planeX = -r->dirY * 0.66;
	r->planeY = r->dirX * 0.66;
	r->screenScale = sqrt((defScreenWidth
				* defScreenHeight) / (1920.0 * 1080.0));
	r->baseMovespeed = 1.0 * r->screenScale;
	r->baseRotSpeed = 0.3 * r->screenScale;
	gettimeofday(&tv, NULL);
	r->lastTime = tv.tv_sec + tv.tv_usec / 1000000.0;
	r->frameTime = 0.016;
	r->moveSpeed = r->baseMovespeed * r->frameTime;
	r->rotSpeed = r->baseRotSpeed * r->frameTime;
	r->oldTime = 0;
	r->currentTime = 0.0;
	return (r);
}

