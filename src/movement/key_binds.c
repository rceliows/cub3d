#include "../inc/cub3d.h"

int	handle_close(t_cub3d *cub3d)
{
	printf("Cleaning and exiting...\n");
	cleanup_cub3d(cub3d);
	exit(0);
	return (0);
}

static void	update_pitch(t_raycaster *r, int delta_y)
{
	r->pitch -= delta_y;
	if (r->pitch > DEFSCREENHEIGHT / 2)
		r->pitch = DEFSCREENHEIGHT / 2;
	if (r->pitch < -DEFSCREENHEIGHT / 2)
		r->pitch = -DEFSCREENHEIGHT / 2;
}

int	mouse_move(int x, int y, t_cub3d *cub3d)
{
	int			delta_x;
	int			delta_y;
	double		rotation;
	t_raycaster	*r;
	t_window	*w;

	r = cub3d->raycaster;
	w = cub3d->window;
	if (!r->keys.mouse_lock)
		return (0);
	delta_x = x - w->center_x;
	if (delta_x != 0)
	{
		rotation = delta_x * 0.002;
		apply_rotation(r, rotation);
	}
	delta_y = y - w->center_y;
	if (delta_y != 0)
		update_pitch(r, delta_y);
	if (r->keys.mouse_lock && (delta_x != 0 || delta_y != 0))
		mlx_mouse_move(w->mlx, w->win, w->center_x, w->center_y);
	return (0);
}

void	process_movement(t_raycaster *r, t_map *map)
{
	if (r->keys.w)
		apply_forward(r, map);
	if (r->keys.s)
		apply_backward(r, map);
	if (r->keys.a)
		apply_strafe_left(r, map);
	if (r->keys.d)
		apply_strafe_right(r, map);
	if (r->keys.right)
		apply_rotation(r, r->rot_speed);
	if (r->keys.left)
		apply_rotation(r, -r->rot_speed);
}

void	prep_hooks(t_cub3d *cub3d)
{
	mlx_hook(cub3d->window->win, 2, 1L << 0, key_press, cub3d);
	mlx_hook(cub3d->window->win, 3, 1L << 1, key_release, cub3d);
	mlx_hook(cub3d->window->win, 17, 1L << 17, handle_close, cub3d);
	mlx_hook(cub3d->window->win, 6, 1L << 6, mouse_move, cub3d);
}
