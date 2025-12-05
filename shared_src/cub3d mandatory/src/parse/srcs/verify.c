/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verify.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 19:44:22 by rceliows          #+#    #+#             */
/*   Updated: 2025/12/05 19:44:25 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	has_xpm_extension(const char *path)
{
	size_t	len;

	if (!path)
		return (0);
	len = ft_strlen(path);
	if (len < 5)
		return (0);
	return (ft_strncmp(path + len - 4, ".xpm", 4) == 0);
}

int	is_valid_texture_path(const char *raw)
{
	int		fd;
	char	*path;

	if (!raw)
		return (0);
	path = (char *)raw;
	while (*path == ' ' || *path == '\t')
		path++;
	if (!has_xpm_extension(path))
		return (0);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (0);
	close(fd);
	return (1);
}

int	validate_textures(t_data *game)
{
	if (!game->north_texture || !game->south_texture || !game->east_texture
		|| !game->west_texture)
	{
		ft_putstr_fd("Error: missing texture paths\n", 2);
		return (0);
	}
	if (!is_valid_texture_path(game->north_texture))
	{
		ft_putstr_fd("Error: invalid NO texture\n", 2);
		return (0);
	}
	if (!is_valid_texture_path(game->south_texture))
		return (ft_putstr_fd("Error: invalid SO texture\n", 2), 0);
	if (!is_valid_texture_path(game->west_texture))
	{
		ft_putstr_fd("Error: invalid WE texture\n", 2);
		return (0);
	}
	if (!is_valid_texture_path(game->east_texture))
	{
		ft_putstr_fd("Error: invalid EA texture\n", 2);
		return (0);
	}
	return (1);
}

int	verify_map(const char *path, t_data *game)
{
	if (!has_cub_extension(path))
	{
		ft_putstr_fd("Proper usage: ./cube3d map.cub\n", 2);
		return (0);
	}
	game->map = parse_map((char *)path, game);
	if (!game->map || !game->map[0])
	{
		ft_putstr_fd("Error: empty map\n", 2);
		return (0);
	}
	if (!validate_textures(game))
	{
		ft_putstr_fd("Error: non textures\n", 2);
		return (0);
	}
	if (!validate_map(game))
	{
		ft_putstr_fd("Error: non valid map\n", 2);
		return (0);
	}
	return (1);
}
