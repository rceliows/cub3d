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
	if (!extract_images_bonus(cub3d->map, cub3d->window))
		error_exit(cub3d);
	if (!init_doors(cub3d->map))
		error_exit(cub3d);
	cub3d->raycaster = init_raycaster(cub3d->window, cub3d->map,
			cub3d->map->start_direction);
	if (!cub3d->raycaster)
		error_exit(cub3d);
	cub3d->raycaster->pitch = 0;
	cub3d->raycaster->keys = *cub3d->keys;
}

static int	alloc_structs(t_cub3d **cub3d, t_data **game)
{
	*cub3d = malloc(sizeof(t_cub3d));
	if (!*cub3d)
		return (0);
	ft_memset(*cub3d, 0, sizeof(t_cub3d));
	*game = malloc(sizeof(t_data));
	if (!*game)
	{
		free(*cub3d);
		return (0);
	}
	ft_memset(*game, 0, sizeof(t_data));
	return (1);
}

static int	setup_game(t_cub3d *cub3d, t_data *game, char *path)
{
	int	height;
	int	width;
	int	exit_code;

	exit_code = verify_map(path, game);
	if (exit_code != 0)
	{
		clenup_game(game);
		cleanup_cub3d(cub3d);
		return (exit_code);
	}
	cub3d->game = game;
	get_map_size(cub3d->game->map, &height, &width);
	cub3d->map = init_map(height, width);
	if (!cub3d->map)
	{
		cleanup_cub3d(cub3d);
		return (1);
	}
	return (0);
}

static void	run_game(t_cub3d *cub3d)
{
	init_cub3d(cub3d);
	prep_hooks(cub3d);
	mlx_mouse_hide(cub3d->window->mlx, cub3d->window->win);
	mlx_loop_hook(cub3d->window->mlx, raycasting_function, cub3d);
	mlx_loop(cub3d->window->mlx);
}

int	main(int ac, char **av)
{
	t_cub3d	*cub3d;
	t_data	*game;
	int		exit_code;

	if (ac != 2)
	{
		printf("Usage: %s map.cub\n", av[0]);
		return (1);
	}
	if (!alloc_structs(&cub3d, &game))
		return (1);
	exit_code = setup_game(cub3d, game, av[1]);
	if (exit_code != 0)
		return (exit_code);
	run_game(cub3d);
	cleanup_cub3d(cub3d);
	return (0);
}
