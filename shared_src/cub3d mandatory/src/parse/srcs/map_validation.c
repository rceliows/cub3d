/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 19:43:23 by rceliows          #+#    #+#             */
/*   Updated: 2025/12/05 19:43:26 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void	flood_fill(char **map_copy, int *player, int *map_size,
				int *params);

static void	fill_directions(char **map_copy, int *player, int *map_size,
		int *params)
{
	int	next_pos[2];
	int	px;
	int	py;

	px = player[0];
	py = player[1];
	next_pos[0] = px + params[0];
	next_pos[1] = py;
	flood_fill(map_copy, next_pos, map_size, params);
	next_pos[0] = px - params[0];
	next_pos[1] = py;
	flood_fill(map_copy, next_pos, map_size, params);
	next_pos[0] = px;
	next_pos[1] = py + params[1];
	flood_fill(map_copy, next_pos, map_size, params);
	next_pos[0] = px;
	next_pos[1] = py - params[1];
	flood_fill(map_copy, next_pos, map_size, params);
}

static void	flood_fill(char **map_copy, int *player, int *map_size, int *params)
{
	int	px;
	int	py;

	px = player[0];
	py = player[1];
	if (px < 0 || py < 0 || px >= map_size[0] || py >= map_size[1])
	{
		params[2] = 0;
		return ;
	}
	if (map_copy[px][py] == '1' || map_copy[px][py] == 'V')
		return ;
	if (map_copy[px][py] == ' ')
	{
		params[2] = 0;
		return ;
	}
	if (map_copy[px][py] == '0' || map_copy[px][py] == 'N'
		|| map_copy[px][py] == 'S' || map_copy[px][py] == 'W'
		|| map_copy[px][py] == 'E')
	{
		map_copy[px][py] = 'V';
		fill_directions(map_copy, player, map_size, params);
	}
}

static void	free_map(char **map, int rows)
{
	int	i;

	i = 0;
	while (i < rows)
	{
		free(map[i]);
		i++;
	}
	free(map);
}

int	find_char_position(char **map, int *map_size, char c, int *position)
{
	int	i;
	int	j;
	int	found;

	found = 0;
	i = 0;
	while (i < map_size[0])
	{
		j = 0;
		while (j < map_size[1])
		{
			if (map[i][j] == c)
			{
				if (found)
					return (0);
				position[0] = i;
				position[1] = j;
				found = 1;
			}
			j++;
		}
		i++;
	}
	return (found);
}

int	map_check(char **map, int *player, int *map_size, int *iteration)
{
	char	**map_copy;
	int		params[3];

	params[0] = iteration[0];
	params[1] = iteration[1];
	params[2] = 1;
	map_copy = copy_map(map, map_size);
	if (!map_copy)
		return (0);
	if (map_copy[player[0]][player[1]] == ' ')
	{
		free_map(map_copy, map_size[0]);
		return (0);
	}
	if (map_copy[player[0]][player[1]] != '0'
		&& map_copy[player[0]][player[1]] != 'N'
		&& map_copy[player[0]][player[1]] != 'S'
		&& map_copy[player[0]][player[1]] != 'W'
		&& map_copy[player[0]][player[1]] != 'E')
		return (free_map(map_copy, map_size[0]), 0);
	flood_fill(map_copy, player, map_size, params);
	free_map(map_copy, map_size[0]);
	return (params[2]);
}
