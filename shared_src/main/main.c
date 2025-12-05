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

static void	init_cub3d(t_cub3d *cub3d)
{
	cub3d->keys = NULL;
	cub3d->window = NULL;
	cub3d->raycaster = NULL;
	cub3d->window = init_window();
	if (!cub3d->window)
		error_exit(cub3d);
	cub3d->keys = init_keys();
	if (!cub3d->keys)
		error_exit(cub3d);
	if (!cub3d->map)
		error_exit(cub3d);
	apply_parsed_to_map(cub3d->map, cub3d->game);
	if (!extract_images(cub3d->map, cub3d->window))
		error_exit(cub3d);
	cub3d->raycaster = init_raycaster(cub3d->window, cub3d->map,
			cub3d->map->start_direction);
	if (!cub3d->raycaster)
		error_exit(cub3d);
	cub3d->raycaster->pitch = 0;
	cub3d->raycaster->keys = *cub3d->keys;
}

#include "../inc/cub3d.h"

int	main(int ac, char **av)
{
	t_cub3d	*cub3d;
	int		height;
	int		width;

	if (ac != 2)
	{
		printf("Usage: %s map.cub\n", av[0]);
		return (1);
	}
	cub3d = malloc(sizeof(t_cub3d));
	if (!cub3d)
		return (1);
	ft_memset(cub3d, 0, sizeof(t_cub3d));
	cub3d->game = verify_map(av[1]);
	if (!cub3d->game)
	{
		cleanup_cub3d(cub3d);
		return (1);
	}
	get_map_size(cub3d->game->map, &height, &width);
	cub3d->map = init_map(height, width);
	if (!cub3d->map)
	{
		cleanup_cub3d(cub3d);
		return (1);
	}
	init_cub3d(cub3d);
	prep_hooks(cub3d);
	mlx_loop_hook(cub3d->window->mlx, raycasting_function, cub3d);
	mlx_loop(cub3d->window->mlx);
	cleanup_cub3d(cub3d);
	return (0);
}

// int	main(int ac, char **av)
// {
// 	t_cub3d	*cub3d;

// 	if (ac != 2)
// 	{
// 		printf("Usage: %s map.cub\n", av[0]);
// 		return (1);
// 	}
// 	cub3d = malloc(sizeof(t_cub3d));
// 	if (!cub3d)
// 		return (1);
// 	ft_memset(cub3d, 0, sizeof(t_cub3d));
// 	if (!verify_map(av[1], &cub3d->game))
// 	{
// 		free(cub3d);
// 		return (1);
// 	}
// 	init_cub3d(cub3d);
// 	prep_hooks(cub3d);
// 	mlx_loop_hook(cub3d->window->mlx, raycasting_function, cub3d);
// 	mlx_loop(cub3d->window->mlx);
// 	cleanup_cub3d(cub3d);
// 	return (0);
// }
// a supression can ocur when using valgrind due to a false flag
// some extra bytes can be added between fields for alignments

// int	main(int argc, char **argv)
// {
// 	t_cub3d	*cub3d;

// 	if (argc != 2)
// 	{
// 		printf("Usage: %s <direction: N/S/E/W>\n", argv[0]);
// 		return (1);
// 	}
// 	cub3d = malloc(sizeof(t_cub3d));
// 	if (!cub3d)
// 		return (1);
// 	init_cub3d(cub3d, *argv[1]);
// 	prep_hooks(cub3d);
// 	mlx_loop_hook(cub3d->window->mlx, raycasting_function, cub3d);
// 	mlx_loop(cub3d->window->mlx);
// 	cleanup_cub3d(cub3d);
// 	return (0);
// }

/*
Main with mouse hidden

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
*/
