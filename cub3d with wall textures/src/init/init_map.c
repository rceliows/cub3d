/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 16:38:29 by rceliows          #+#    #+#             */
/*   Updated: 2025/10/29 16:38:30 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int		extract_images(t_map *map, t_window *w)
{
	int	width;
	int	height;
	
	map->texture[0] = mlx_xpm_file_to_image(w->mlx, map->n_xpm_path, &width, &height);
	if (!map->n_image)
		return (0);
	map->texture[1] = mlx_xpm_file_to_image(w->mlx, map->s_xpm_path, &width, &height);
	if (!map->s_image)
		return (0);
	map->texture[3] = mlx_xpm_file_to_image(w->mlx, map->e_xpm_path, &width, &height);
	if (!map->e_image)
		return (0);
	map->texture[2] = mlx_xpm_file_to_image(w->mlx, map->w_xpm_path, &width, &height);
	if (!map->w_image)
		return (0);
	return (1);
}

t_map	*init_map(t_window *w)
{
	t_map	*map;

	map = malloc(sizeof(t_map));
	if (!map)
		return (NULL);
	int	templatemap[MAPHEIGHT][MAPWIDTH]=
	{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,0,1,0,1,0,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1},
	{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,0,1,0,1,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	for (int i = 0; i < MAPHEIGHT; i++)
	{
		for (int j = 0; j < MAPWIDTH; j++)
			map->world_map[i][j] = templatemap[i][j];
	}
	map->n_xpm_path = ft_strdup("./textures/n_image.xpm");
	map->s_xpm_path = ft_strdup("./textures/s_image.xpm");
	map->e_xpm_path = ft_strdup("./textures/e_image.xpm");
	map->w_xpm_path = ft_strdup("./textures/w_image.xpm");
	if (!extract_images(map, w))
		return (NULL);
	map->ceiling_color = 0x87CEEB;
	map->floor_color = 0x404040;
	map->start_position_x = 22.0;
	map->start_position_y = 12.0;
	map->start_direction = 0;
	return(map);
}

void    cleanup_map(t_map *map, t_window *w)
{
	if (map->n_image)
		mlx_destroy_image(w->mlx, map->n_image);
	if (map->s_image)
		mlx_destroy_image(w->mlx, map->s_image);
	if (map->e_image)
		mlx_destroy_image(w->mlx, map->e_image);
	if (map->w_image)
		mlx_destroy_image(w->mlx, map->w_image);
	if (map->n_xpm_path)
		free(map->n_xpm_path);
	if (map->s_xpm_path)
		free(map->s_xpm_path);
	if (map->e_xpm_path)
		free(map->e_xpm_path);
	if (map->w_xpm_path)
		free(map->w_xpm_path);
}