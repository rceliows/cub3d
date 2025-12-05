/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_doors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:59:21 by rceliows          #+#    #+#             */
/*   Updated: 2025/11/05 15:59:22 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	count_doors(t_map *map)
{
	int	door_count;
	int	i;
	int	j;

	door_count = 0;
	i = 0;
	while (i < map->map_height)
	{
		j = 0;
		while (j < map->map_width)
		{
			if (map->world_map[i][j] == 2)
				door_count++;
			j++;
		}
		i++;
	}
	return (door_count);
}

static void	init_manager(t_door_manager *mgr, t_map *map)
{
	int	i;
	int	j;

	i = 0;
	while (i < map->map_height)
	{
		j = 0;
		while (j < map->map_width)
		{
			if (map->world_map[i][j] == 2)
			{
				mgr->doors[mgr->count].x = i;
				mgr->doors[mgr->count].y = j;
				mgr->doors[mgr->count].state = 0;
				mgr->doors[mgr->count].timer = 0;
				mgr->count++;
			}
			j++;
		}
		i++;
	}
}

int	init_doors(t_map *map)
{
	t_door_manager	*mgr;

	mgr = get_door_manager();
	mgr->capacity = count_doors(map);
	mgr->count = 0;
	mgr->doors = malloc(sizeof(t_door) * mgr->capacity);
	if (!mgr->doors)
		return (0);
	init_manager(mgr, map);
	return(1);
}

void	cleanup_doors(void)
{
	t_door_manager	*mgr;

	mgr = get_door_manager();
	if (mgr->doors)
	{
		free(mgr->doors);
		mgr->doors = NULL;
		mgr->count = 0;
		mgr->capacity = 0;
	}
}
