#include "../inc/cub3d.h"

int	key_press_bonus(int keycode, t_raycaster *r)
{
	if (keycode == 102)
		r->keys.fps = !r->keys.fps;
	else if (keycode == 109)
		r->keys.minimap = !r->keys.minimap;
	else if (keycode == 113)
		r->keys.mouse_lock = !r->keys.mouse_lock;
	else
		return (0);
	return (1);
}

int	key_press_movement(int keycode, t_raycaster *r)
{
	if (keycode == 119)
		r->keys.w = 1;
	else if (keycode == 97)
		r->keys.a = 1;
	else if (keycode == 115)
		r->keys.s = 1;
	else if (keycode == 100)
		r->keys.d = 1;
	else if (keycode == 65361)
		r->keys.left = 1;
	else if (keycode == 65363)
		r->keys.right = 1;
	else if (keycode == 101)
		r->keys.e = 1;
	else
		return (0);
	return (1);
}

int	key_press(int keycode, t_cub3d *cub3d)
{
	t_raycaster	*r;

	r = cub3d->raycaster;
	if (key_press_bonus(keycode, r))
		return (0);
	if (key_press_movement(keycode, r))
	{
		if (keycode == 101)
			toggle_door(r, cub3d->map);
		return (0);
	}
	if (keycode == 65307)
	{
		printf("ESC pressed, exiting...\n");
		cleanup_cub3d(cub3d);
		exit(0);
	}
	return (0);
}

int	key_release_movement(int keycode, t_raycaster *r)
{
	if (keycode == 119)
		r->keys.w = 0;
	else if (keycode == 97)
		r->keys.a = 0;
	else if (keycode == 115)
		r->keys.s = 0;
	else if (keycode == 100)
		r->keys.d = 0;
	else if (keycode == 65361)
		r->keys.left = 0;
	else if (keycode == 65363)
		r->keys.right = 0;
	else if (keycode == 101)
		r->keys.e = 0;
	else
		return (0);
	return (1);
}

int	key_release(int keycode, t_cub3d *cub3d)
{
	t_raycaster	*r;

	r = cub3d->raycaster;
	key_release_movement(keycode, r);
	return (0);
}
