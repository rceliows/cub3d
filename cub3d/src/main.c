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
	init_keys(&cub3d->keys);
	init_map(&cub3d->map);
	if (!init_window(&cub3d->window, &cub3d->raycaster))
		error_exit(cub3d);
	init_raycaster(&cub3d->raycaster, &cub3d->window, direction);
}

int	main(int argc, char **argv)
{
	t_cub3d	cub3d;
	
	if (argc != 2)
	{
		printf("Insert input\n");
		return (1);
	}
	init_cub3d(&cub3d, *argv[1]);
	prep_hooks(&cub3d.raycaster, &cub3d.window);
	mlx_mouse_hide(cub3d.window.mlx, cub3d.window.win);
//	mlx_loop_hook(cub3d.window.mlx, raycasting_function, &cub3d.raycaster);
	mlx_loop_hook(cub3d.window.mlx, raycasting_function, &cub3d);
	mlx_loop(cub3d.window.mlx);
	cleanup_cub3d(&cub3d);
	return (0);
}