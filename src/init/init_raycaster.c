#include "../inc/cub3d.h"

void	setup_direction(t_raycaster *r, int direction)
{
	if (direction == 'N')
	{
		r->dir_x = 0;
		r->dir_y = -1;
	}
	else if (direction == 'S')
	{
		r->dir_x = 0;
		r->dir_y = 1;
	}
	else if (direction == 'E')
	{
		r->dir_x = 1;
		r->dir_y = 0;
	}
	else if (direction == 'W')
	{
		r->dir_x = -1;
		r->dir_y = 0;
	}
}

t_raycaster	*init_raycaster(t_window *w, t_map *map, int direction)
{
	t_raycaster		*r;
	struct timeval	tv;

	r = malloc(sizeof(t_raycaster));
	if (!r)
		return (NULL);
	r->img_data = mlx_get_data_addr(w->img,
			&r->bits_per_pixel, &r->line_length, &r->endian);
	r->pos_x = map->start_position_x;
	r->pos_y = map->start_position_y;
	setup_direction(r, direction);
	r->plane_x = -r->dir_y * 0.66;
	r->plane_y = r->dir_x * 0.66;
	r->ceiling_color = map->ceiling_color;
	r->floor_color = map->floor_color;
	r->screen_scale = sqrt((DEFSCREENWIDTH
				* DEFSCREENHEIGHT) / (1920.0 * 1080.0));
	r->base_move_speed = 1.0 * r->screen_scale;
	r->base_rot_speed = 0.3 * r->screen_scale;
	gettimeofday(&tv, NULL);
	r->last_time = tv.tv_sec + tv.tv_usec / 1000000.0;
	r->frame_time = 0.016;
	r->move_speed = r->base_move_speed * r->frame_time;
	r->rot_speed = r->base_rot_speed * r->frame_time;
	return (r);
}
