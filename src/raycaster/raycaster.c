#include "../inc/cub3d.h"

static void	draw_floor_ceiling(t_raycaster *r, t_raycaster_var *v)
{
	int	y;

	y = 0;
	while (y < v->draw_start)
	{
		if (y >= 0 && y < DEFSCREENHEIGHT)
			set_pixel(v->x, y, r->ceiling_color, r);
		y++;
	}
	y = v->draw_end;
	while (y < DEFSCREENHEIGHT)
	{
		if (y >= 0 && y < DEFSCREENHEIGHT)
			set_pixel(v->x, y, r->floor_color, r);
		y++;
	}
}

static void	draw_walls(t_raycaster *r, t_raycaster_var *v, void *texture)
{
	int				y;
	int				d;
	unsigned int	color;

	calculate_texture_x(r, v);
	y = v->draw_start;
	while (y < v->draw_end)
	{
		if (y >= 0 && y < DEFSCREENHEIGHT)
		{
			d = (y - (DEFSCREENHEIGHT / 2 + r->pitch))
				* 256 + (v->line_height * 128);
			v->tex_y = ((d * TEXTURESIZE) / v->line_height) / 256;
			if (v->tex_y < 0)
				v->tex_y = 0;
			if (v->tex_y >= TEXTURESIZE)
				v->tex_y = TEXTURESIZE - 1;
			color = get_texture_pixel(texture, v, v->tex_width, v->tex_height);
			if (v->side == 1)
				color = (color >> 1) & 0x7F7F7F;
			set_pixel(v->x, y, color, r);
		}
		y++;
	}
}

static int	check_direction(t_raycaster_var *v)
{
	if (v->side == 0)
	{
		if (v->step_x == -1)
			return (2);
		else
			return (3);
	}
	else
	{
		if (v->step_y == -1)
			return (0);
		else
			return (1);
	}
}

static void	draw_image(t_raycaster *r, t_raycaster_var *v, t_map *map)
{
	int		direction;
	int		cell_type;

	draw_floor_ceiling(r, v);
	cell_type = map->world_map[v->map_x][v->map_y];
	if (cell_type == 2)
	{
		direction = 4;
		v->tex_width = map->texture_width[4];
		v->tex_height = map->texture_height[4];
		draw_walls(r, v, map->texture[4]);
		return ;
	}
	direction = check_direction(v);
	v->tex_width = map->texture_width[direction];
	v->tex_height = map->texture_height[direction];
	draw_walls(r, v, map->texture[direction]);
}

int	raycasting_function(t_cub3d *cub3d)
{
	t_raycaster_var	v;
	t_raycaster		*r;
	t_window		*w;
	t_map			*map;

	r = cub3d->raycaster;
	w = cub3d->window;
	map = cub3d->map;
	update_time_and_speed(r);
	update_doors(r, map);
	process_movement(r, map);
	v.x = 0;
	while (v.x < DEFSCREENWIDTH)
	{
		calculate_ray(r, &v);
		calculate_side_distance(r, &v);
		perform_dda(map, &v);
		calculate_line(&v, r);
		draw_image(r, &v, map);
		v.x++;
	}
	render_sprites(cub3d, v.z_buffer);
	mlx_put_image_to_window(w->mlx, w->win, w->img, 0, 0);
	bonus_elements(r, w, map);
	return (0);
}
