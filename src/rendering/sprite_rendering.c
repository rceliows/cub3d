#include "../inc/cub3d.h"

static void	sort_sprites(t_sprite_data *sprites)
{
	int		i;
	int		j;
	int		tmp_order;
	double	tmp_dist;

	i = 0;
	while (i < sprites->count - 1)
	{
		j = 0;
		while (j < sprites->count - i - 1)
		{
			if (sprites->distance[j] < sprites->distance[j + 1])
			{
				tmp_dist = sprites->distance[j];
				sprites->distance[j] = sprites->distance[j + 1];
				sprites->distance[j + 1] = tmp_dist;
				tmp_order = sprites->order[j];
				sprites->order[j] = sprites->order[j + 1];
				sprites->order[j + 1] = tmp_order;
			}
			j++;
		}
		i++;
	}
}

static void	calculate_sprite_variables(t_raycaster *r, t_sprite_var *vs,
		t_sprite_data *s, int i)
{
	vs->sprite_x = s->pos_x[s->order[i]] - r->pos_x;
	vs->sprite_y = s->pos_y[s->order[i]] - r->pos_y;
	vs->inv_det = 1.0 / (r->plane_x * r->dir_y - r->dir_x * r->plane_y);
	vs->transform_x = vs->inv_det * (r->dir_y * vs->sprite_x
			- r->dir_x * vs->sprite_y);
	vs->transform_y = vs->inv_det * (-r->plane_y * vs->sprite_x
			+ r->plane_x * vs->sprite_y);
	vs->sprite_screen_x = (int)((DEFSCREENWIDTH / 2)
			* (1 + vs->transform_x / vs->transform_y));
	vs->sprite_height = abs((int)(DEFSCREENHEIGHT / vs->transform_y));
	vs->sprite_width = abs((int)(DEFSCREENHEIGHT / vs->transform_y));
	vs->draw_start_y = -vs->sprite_height / 2 + DEFSCREENHEIGHT / 2 + r->pitch;
	vs->draw_end_y = vs->sprite_height / 2 + DEFSCREENHEIGHT / 2 + r->pitch;
}

static void	calculate_sprite_params(t_sprite_var *vs)
{
	vs->draw_params[0] = -vs->sprite_width / 2 + vs->sprite_screen_x;
	vs->draw_params[1] = vs->sprite_width / 2 + vs->sprite_screen_x;
	vs->draw_params[2] = vs->draw_start_y;
	vs->draw_params[3] = vs->sprite_height;
	vs->draw_params[4] = -vs->sprite_width / 2 + vs->sprite_screen_x;
	vs->draw_params[5] = vs->draw_end_y;
	vs->draw_params[6] = vs->transform_y;
	vs->draw_params[7] = vs->sprite_screen_x;
	if (vs->draw_params[0] < 0)
		vs->draw_params[0] = 0;
	if (vs->draw_params[1] >= DEFSCREENWIDTH)
		vs->draw_params[1] = DEFSCREENWIDTH - 1;
	if (vs->draw_params[2] < 0)
		vs->draw_params[2] = 0;
	if (vs->draw_params[5] >= DEFSCREENHEIGHT)
		vs->draw_params[5] = DEFSCREENHEIGHT - 1;
}

void	render_sprites(t_cub3d *cub3d, double *z_buffer)
{
	t_sprite_data	sprites;
	t_sprite_var	vs;
	int				i;

	if (!collect_sprites(cub3d->map, cub3d->raycaster, &sprites)
		|| sprites.count == 0)
	{
		cleanup_sprites(&sprites);
		return ;
	}
	sort_sprites(&sprites);
	vs.z_buffer = z_buffer;
	i = 0;
	while (i < sprites.count)
	{
		calculate_sprite_variables(cub3d->raycaster, &vs, &sprites, i);
		calculate_sprite_params(&vs);
		draw_sprite_stripe(cub3d, &sprites, sprites.order[i],
			&vs);
		i++;
	}
	cleanup_sprites(&sprites);
}
