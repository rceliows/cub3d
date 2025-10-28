/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 15:09:04 by rceliows          #+#    #+#             */
/*   Updated: 2025/10/28 15:09:05 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	init_raycaster(t_raycaster *raycaster)
{
	raycaster->mlx = mlx_init();
	if (!raycaster->mlx)
		error_exit(raycaster);
	raycaster->win = mlx_new_window(raycaster->mlx, screenWidth, screenHeight, "cub3d");
	if (!raycaster->win)
		error_exit(raycaster);
	raycaster->img = mlx_new_image(raycaster->mlx, screenWidth, screenHeight);
	if (!raycaster->img)
		error_exit(raycaster);
	raycaster->img_data = mlx_get_data_addr(raycaster->img, &raycaster->bits_per_pixel, &raycaster->line_length, &raycaster->endian);
}

void	cleanup_raycaster(t_raycaster *raycaster)
{
	if (raycaster->img)
	{
		mlx_destroy_image(raycaster->mlx, raycaster->img);
		raycaster->img = NULL;
	}
	if (raycaster->win)
	{
		mlx_destroy_window(raycaster->mlx, raycaster->win);
		raycaster->win = NULL;
	}
	if (raycaster->mlx)
	{
		mlx_destroy_display(raycaster->mlx);
		free(raycaster->mlx);
		raycaster->mlx = NULL;
	}
}

void	error_exit(t_raycaster *raycaster)
{
	cleanup_raycaster(raycaster);
	exit(1);
}
