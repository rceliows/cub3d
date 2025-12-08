/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_rendering.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 13:28:02 by rceliows          #+#    #+#             */
/*   Updated: 2025/11/04 13:28:03 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static t_door	*find_nearest_door(t_raycaster *r)
{
	t_door_manager	*mgr;
	t_door			*nearest;
	double			min_dist;
	double			dist;
	int				i;

	mgr = get_door_manager();
	nearest = NULL;
	min_dist = 2.0;
	i = 0;
	while (i < mgr->count)
	{
		dist = sqrt((r->pos_x - mgr->doors[i].x - 0.5)
				* (r->pos_x - mgr->doors[i].x - 0.5)
				+ (r->pos_y - mgr->doors[i].y - 0.5)
				* (r->pos_y - mgr->doors[i].y - 0.5));
		if (dist < min_dist)
		{
			min_dist = dist;
			nearest = &mgr->doors[i];
		}
		i++;
	}
	return (nearest);
}

void	toggle_door(t_raycaster *r, t_map *map)
{
	t_door	*door;

	door = find_nearest_door(r);
	if (door)
	{
		door->state = !door->state;
		door->timer = 0;
		if (door->state)
			map->world_map[door->x][door->y] = 0;
		else
			map->world_map[door->x][door->y] = 2;
	}
}

static int	is_player_in_door(t_raycaster *r, t_door *door)
{
	int		player_x;
	int		player_y;

	player_x = (int)r->pos_x;
	player_y = (int)r->pos_y;
	if (player_x == door->x && player_y == door->y)
		return (1);
	return (0);
}

void	update_doors(t_raycaster *r, t_map *map)
{
	t_door_manager	*mgr;
	int				i;

	mgr = get_door_manager();
	i = 0;
	while (i < mgr->count)
	{
		if (mgr->doors[i].state == 1)
		{
			mgr->doors[i].timer += r->frame_time;
			if (mgr->doors[i].timer >= 3.0
				&& !is_player_in_door(r, &mgr->doors[i]))
			{
				mgr->doors[i].state = 0;
				mgr->doors[i].timer = 0;
				map->world_map[mgr->doors[i].x][mgr->doors[i].y] = 2;
			}
		}
		i++;
	}
}

int	get_door_state(int x, int y)
{
	t_door_manager	*mgr;
	int				i;

	mgr = get_door_manager();
	i = 0;
	while (i < mgr->count)
	{
		if (mgr->doors[i].x == x && mgr->doors[i].y == y)
			return (mgr->doors[i].state);
		i++;
	}
	return (0);
}
