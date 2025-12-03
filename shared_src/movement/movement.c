/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:24:23 by rceliows          #+#    #+#             */
/*   Updated: 2025/11/05 15:24:24 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	apply_rotation(t_raycaster *r, double angle)
{
	double	olddir_x;
	double	oldplane_x;

	olddir_x = r->dir_x;
	r->dir_x = r->dir_x * cos(angle) - r->dir_y * sin(angle);
	r->dir_y = olddir_x * sin(angle) + r->dir_y * cos(angle);
	oldplane_x = r->plane_x;
	r->plane_x = r->plane_x * cos(angle) - r->plane_y * sin(angle);
	r->plane_y = oldplane_x * sin(angle) + r->plane_y * cos(angle);
}

void	apply_forward(t_raycaster *r, t_map *map)
{
	if (map->world_map[(int)(r->pos_x + r->dir_x * r->move_speed)]
		[(int)(r->pos_y)] == 0)
		r->pos_x += r->dir_x * r->move_speed;
	if (map->world_map[(int)(r->pos_x)]
		[(int)(r->pos_y + r->dir_y * r->move_speed)] == 0)
		r->pos_y += r->dir_y * r->move_speed;
}

void	apply_backward(t_raycaster *r, t_map *map)
{
	if (map->world_map[(int)(r->pos_x - r->dir_x * r->move_speed)]
		[(int)(r->pos_y)] == 0)
		r->pos_x -= r->dir_x * r->move_speed;
	if (map->world_map[(int)(r->pos_x)]
		[(int)(r->pos_y - r->dir_y * r->move_speed)] == 0)
		r->pos_y -= r->dir_y * r->move_speed;
}

void	apply_strafe_right(t_raycaster *r, t_map *map)
{
	if (map->world_map[(int)(r->pos_x - r->dir_y * r->move_speed)]
		[(int)(r->pos_y)] == 0)
		r->pos_x -= r->dir_y * r->move_speed;
	if (map->world_map[(int)(r->pos_x)]
		[(int)(r->pos_y + r->dir_x * r->move_speed)] == 0)
		r->pos_y += r->dir_x * r->move_speed;
}

void	apply_strafe_left(t_raycaster *r, t_map *map)
{
	if (map->world_map[(int)(r->pos_x + r->dir_y * r->move_speed)]
		[(int)(r->pos_y)] == 0)
		r->pos_x += r->dir_y * r->move_speed;
	if (map->world_map[(int)(r->pos_x)]
		[(int)(r->pos_y - r->dir_x * r->move_speed)] == 0)
		r->pos_y -= r->dir_x * r->move_speed;
}