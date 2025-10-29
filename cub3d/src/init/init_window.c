/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_window.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 16:37:57 by rceliows          #+#    #+#             */
/*   Updated: 2025/10/29 16:37:59 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int    init_window(t_window *window, t_raycaster *raycaster)
{
    window->mlx = mlx_init();
	if (!window->mlx)
		return(1);
//	mlx_get_screen_size(raycaster->mlx, &raycaster->screenWidth, &raycaster->screenHeight);
	raycaster->screenWidth = defaultScreenWidth;
	raycaster->screenHeight = defaultScreenHeight;
	window->win = mlx_new_window(window->mlx, raycaster->screenWidth, raycaster->screenHeight, "cub3d");
	if (!window->win)
		return(1);
	window->img = mlx_new_image(window->mlx, raycaster->screenWidth, raycaster->screenHeight);
	if (!window->img)
		return(1);
    return(0);
}


void	cleanup_window(t_window *window)
{
	if (window->img)
	{
		mlx_destroy_image(window->mlx, window->img);
		window->img = NULL;
	}
	if (window->win)
	{
		mlx_destroy_window(window->mlx, window->win);
		window->win = NULL;
	}
	if (window->mlx)
	{
		mlx_destroy_display(window->mlx);
		free(window->mlx);
		window->mlx = NULL;
	}
}