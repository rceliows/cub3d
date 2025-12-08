#include "../inc/cub3d.h"

void	calculate_line(t_raycaster_var *v, t_raycaster *r)
{
	if (v->side == 0)
		v->perp_wall_dist = (v->side_dist_x - v->delta_dist_x);
	else
		v->perp_wall_dist = (v->side_dist_y - v->delta_dist_y);
	if (v->perp_wall_dist < 0.01)
		v->perp_wall_dist = 0.01;
	v->line_height = (int)(DEFSCREENHEIGHT / v->perp_wall_dist);
	if (v->line_height > DEFSCREENHEIGHT * 10)
		v->line_height = DEFSCREENHEIGHT * 10;
	v->draw_start = -v->line_height / 2 + DEFSCREENHEIGHT / 2 + r->pitch;
	if (v->draw_start < 0)
		v->draw_start = 0;
	v->draw_end = v->line_height / 2 + DEFSCREENHEIGHT / 2 + r->pitch;
	if (v->draw_end >= DEFSCREENHEIGHT)
		v->draw_end = DEFSCREENHEIGHT - 1 ;
	v->z_buffer[v->x] = v->perp_wall_dist;
}

void	calculate_side_distance(t_raycaster *r, t_raycaster_var *v)
{
	if (v->ray_dir_x < 0)
	{
		v->step_x = -1;
		v->side_dist_x = (r->pos_x - v->map_x) * v->delta_dist_x;
	}
	else
	{
		v->step_x = 1;
		v->side_dist_x = (v->map_x + 1.0 - r->pos_x) * v->delta_dist_x;
	}
	if (v->ray_dir_y < 0)
	{
		v->step_y = -1;
		v->side_dist_y = (r->pos_y - v->map_y) * v->delta_dist_y;
	}
	else
	{
		v->step_y = 1;
		v->side_dist_y = (v->map_y + 1.0 - r->pos_y) * v->delta_dist_y;
	}
}

void	calculate_ray(t_raycaster *r, t_raycaster_var *v)
{
	v->camera_x = 2 * v->x / (double)DEFSCREENWIDTH - 1;
	v->ray_dir_x = r->dir_x + r->plane_x * v->camera_x;
	v->ray_dir_y = r->dir_y + r->plane_y * v->camera_x;
	v->map_x = (int)(r->pos_x);
	v->map_y = (int)(r->pos_y);
	if (v->ray_dir_x == 0)
		v->delta_dist_x = 1e30;
	else
		v->delta_dist_x = fabs(1 / v->ray_dir_x);
	if (v->ray_dir_y == 0)
		v->delta_dist_y = 1e30;
	else
		v->delta_dist_y = fabs(1 / v->ray_dir_y);
}

void	calculate_texture_x(t_raycaster *r, t_raycaster_var *v)
{
	double	wall_x;

	if (v->side == 0)
		wall_x = r->pos_y + v->perp_wall_dist * v->ray_dir_y;
	else
		wall_x = r->pos_x + v->perp_wall_dist * v->ray_dir_x;
	wall_x -= floor(wall_x);
	v->tex_x = (int)(wall_x * (double)TEXTURESIZE);
	if (v->side == 0 && v->ray_dir_x < 0)
		v->tex_x = TEXTURESIZE - v->tex_x - 1;
	if (v->side == 1 && v->ray_dir_y > 0)
		v->tex_x = TEXTURESIZE - v->tex_x - 1;
	if (v->tex_x < 0)
		v->tex_x = 0;
	if (v->tex_x >= TEXTURESIZE)
		v->tex_x = TEXTURESIZE - 1;
}
