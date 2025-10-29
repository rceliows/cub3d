/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 15:09:19 by rceliows          #+#    #+#             */
/*   Updated: 2025/10/28 15:09:22 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static void	init_cub3d(t_cub3d *cub3d, int direction)
{
	cub3d->keys = NULL;
	cub3d->map = NULL;
	cub3d->window = NULL;
	cub3d->raycaster = NULL;
	cub3d->keys = init_keys();
	if (!cub3d->keys)
		error_exit(cub3d);
	cub3d->map = init_map();
	if (!cub3d->map)
		error_exit(cub3d);
	cub3d->window = init_window();
	if (!cub3d->window)
		error_exit(cub3d);
	cub3d->raycaster = init_raycaster(cub3d->window, cub3d->map, direction);
	if (!cub3d->raycaster)
		error_exit(cub3d);
}

int	main(int argc, char **argv)
{
	t_cub3d	*cub3d;

	if (argc != 2)
	{
		printf("Usage: %s <direction: N/S/E/W>\n", argv[0]);
		return (1);
	}
	cub3d = malloc(sizeof(t_cub3d));
	if (!cub3d)
		return (1);
	init_cub3d(cub3d, *argv[1]);
	prep_hooks(cub3d);
	mlx_mouse_hide(cub3d->window->mlx, cub3d->window->win);
	mlx_loop_hook(cub3d->window->mlx, raycasting_function, cub3d);
	mlx_loop(cub3d->window->mlx);
	cleanup_cub3d(cub3d);
	return (0);
}
