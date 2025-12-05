#ifndef CUBE3D_H
# define CUBE3D_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_data
{
	char	**map;
	char	*north_texture;
	char	*south_texture;
	char	*west_texture;
	char	*east_texture;
	int		floor[3];
	int		ceiling[3];
	int		have_floor;
	int		have_ceiling;
	char player_dir;
	int player_x;
	int player_y;
}	t_data;

// helpers
void rstrip_nl(char *s);
int	validate_map(t_data *game);
void	ft_free_split(char **arr);
int	map_check(char **map, int *player, int *map_size, int *iteration);
int	find_char_position(char **map, int *map_size, char c, int *position);

#endif
