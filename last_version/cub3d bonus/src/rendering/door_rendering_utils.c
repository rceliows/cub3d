/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_rendering_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:58:43 by rceliows          #+#    #+#             */
/*   Updated: 2025/11/05 15:58:45 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

t_door_manager	*get_door_manager(void)
{
	static t_door_manager	manager = {NULL, 0, 0};

	return (&manager);
}

int	is_door(t_map *map, int x, int y)
{
	if (x < 0 || x >= map->map_height || y < 0 || y >= map->map_width)
		return (0);
	return (map->world_map[x][y] == 2);
}
