/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_pos.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 19:44:11 by rceliows          #+#    #+#             */
/*   Updated: 2025/12/05 19:44:17 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	get_map_bounds(char **map, int map_size[2], int iteratio[2])
{
	int	row;
	int	max_col;
	int	len;

	row = 0;
	max_col = 0;
	while (map[row])
	{
		len = (int)ft_strlen(map[row]);
		if (len > max_col)
			max_col = len;
		row++;
	}
	if (row == 0)
	{
		ft_putstr_fd("Error: empty map\n", 2);
		return (0);
	}
	map_size[0] = row;
	map_size[1] = max_col;
	iteratio[0] = 1;
	iteratio[1] = 1;
	return (1);
}

static void	scan_player(char **map, int size[2], int player[2], char *dir_info)
{
	int	y;
	int	x;
	int	count;

	count = 0;
	y = 0;
	while (y < size[0])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == 'N' || map[y][x] == 'S' || map[y][x] == 'E'
				|| map[y][x] == 'W')
			{
				player[0] = y;
				player[1] = x;
				*dir_info = map[y][x];
				count++;
			}
			x++;
		}
		y++;
	}
	dir_info[1] = count;
}

static int	validate_player(int count)
{
	if (count == 0)
	{
		ft_putstr_fd("Error: no player found\n", 2);
		return (0);
	}
	if (count > 1)
	{
		ft_putstr_fd("Error: multiple player found\n", 2);
		return (0);
	}
	return (1);
}

static int	find_player_position(char **map, int size[2], int player[2],
		char *dir)
{
	char	info[2];

	info[0] = 0;
	info[1] = 0;
	scan_player(map, size, player, info);
	*dir = info[0];
	return (validate_player(info[1]));
}

int	validate_map(t_data *game)
{
	int		map_size[2];
	int		player[2];
	int		iteratio[2];
	char	dir;

	if (!get_map_bounds(game->map, map_size, iteratio))
		return (0);
	dir = 0;
	if (!find_player_position(game->map, map_size, player, &dir))
		return (0);
	if (!map_check(game->map, player, map_size, iteratio))
	{
		ft_putstr_fd("Error: fails map check\n", 2);
		return (0);
	}
	game->player_y = player[0];
	game->player_x = player[1];
	game->player_dir = dir;
	return (1);
}
