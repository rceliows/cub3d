#include "../inc/cub3d.h"

void	get_map_size(char **map, int *height, int *width)
{
	int	h;
	int	max_w;
	int	len;

	h = 0;
	max_w = 0;
	while (map[h])
	{
		len = (int)ft_strlen(map[h]);
		if (len > max_w)
			max_w = len;
		h++;
	}
	*height = h;
	*width = max_w;
}

static int	char_to_tile(char c)
{
	if (c == '1')
		return (1);
	else if (c == '0' || c == 'N' || c == 'S' || c == 'W' || c == 'E')
		return (0);
	else if (c == ' ')
		return (1);
	return (1);
}

static int	pack_rbg(int rgb[3])
{
	return ((rgb[0] << 16) | (rgb[1] << 8) | rgb[2]);
}

//  static int dir_char_to_int(char c)
//  {
// 	if (c == 'N')
// 		return (0);
// 	if (c == 'E')
// 		return (1);
// 	if (c == 'S')
// 		return (2);
// 	if (c == 'W')
// 		return (3);
// 	return (0);
//  }
void	apply_parsed_to_map(t_map *m, t_data *g)
{
	int	y;
	int	x;
	int	len;

	m->n_xpm_path = ft_strdup(g->North_texture);
	m->s_xpm_path = ft_strdup(g->South_texture);
	m->w_xpm_path = ft_strdup(g->West_texture);
	m->e_xpm_path = ft_strdup(g->East_texture);
	m->floor_color = pack_rbg(g->floor);
	m->ceiling_color = pack_rbg(g->ceiling);
	m->start_position_x = (double)g->player_x;
	m->start_position_y = (double)g->player_y;
	m->start_direction = g->player_dir;
	y = 0;
	while (y < m->map_height)
	{
		len = (int)ft_strlen(g->map[y]);
		x = 0;
		while (x < m->map_width)
		{
			if (x < len)
				m->world_map[y][x] = char_to_tile(g->map[y][x]);
			else
				m->world_map[y][x] = 1;
			x++;
		}
		y++;
	}
	m->world_map[g->player_y][g->player_x] = 0;
}
