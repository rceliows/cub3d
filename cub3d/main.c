#include "../minilibx-linux/mlx.h"
#include <stdlib.h>
#include <stdio.h>

#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480

typedef struct s_raycaster
{
	int	bits_per_pixel;
	int	line_length;
	int	endian;
	void	*mlx;
	void	*win;
	void	*img;
	double	imag;
	char	*img_data;
}	t_raycaster;

int worldMap[mapWidth][mapHeight]=
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

int	handle_close(t_raycaster *raycaster)
{
	printf("Cleaning and exiting...\n");
	cleanup_raycaster(raycaster);
	exit (0);
}

int	handle_esc(int keycode, t_raycaster *raycaster)
{
	if (keycode == 65307)
	{
		printf("ESC pressed, exiting...\n");
		cleanup_raycaster(raycaster);
		exit(0);
	}
	return (0);
}

int	key_dispatcher(int keycode, t_raycaster *raycaster)
{
	if (keycode == 65307)
		return (handle_esc(keycode, raycaster));
/*
	else if (keycode == 65361 || keycode == 65363
		|| keycode == 65362 || keycode == 65364
		|| keycode == 97 || keycode == 100
		|| keycode == 119 || keycode == 115)
		return (handle_arrows(keycode, raycaster));
*/
	return (0);
}



void	prep_hooks(t_raycaster *raycaster)
{
	mlx_hook(raycaster->win, 2, 1L << 0, key_dispatcher, raycaster);
	mlx_hook(raycaster->win, 17, 1L << 17, handle_close, raycaster);
}

int	main(void)
{
	t_raycaster	raycaster;
	
	init_raycaster(&raycaster);
	prep_hooks(&raycaster);
	mlx_loop(&raycaster.mlx);
	cleanup_raycaster(&raycaster);
	return (0);
}
