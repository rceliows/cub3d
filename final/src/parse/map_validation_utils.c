/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:29:19 by rceliows          #+#    #+#             */
/*   Updated: 2025/12/07 16:29:20 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

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
