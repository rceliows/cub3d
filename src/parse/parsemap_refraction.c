#include "../inc/cub3d.h"

void	copy_old_rows(char ***map, char **new_map)
{
	int	i;

	i = 0;
	while ((*map)[i])
	{
		new_map[i] = (*map)[i];
		i++;
	}
	new_map[i] = NULL;
	free(*map);
}

int	ensure_cap(char ***map, int *cap, int need)
{
	char	**new_map;

	if (need + 1 <= *cap)
		return (0);
	if (*cap == 0)
		*cap = 8;
	else
		*cap = *cap * 2;
	while (*cap < need + 1)
		*cap = *cap * 2;
	new_map = malloc(sizeof(char *) * (*cap + 1));
	if (!new_map)
		return (1);
	if (*map)
		copy_old_rows(map, new_map);
	else
		new_map[0] = NULL;
	*map = new_map;
	return (0);
}

int	push_map_line(char ***map, int *map_count, int *map_cap, char *line)
{
	rstrip_nl(line);
	if (is_blank(line))
		return (1);
	if (ensure_cap(map, map_cap, *map_count + 1))
		return (1);
	(*map)[*map_count] = ft_strdup(line);
	if (!(*map)[*map_count])
		return (1);
	(*map_count)++;
	(*map)[*map_count] = NULL;
	return (0);
}
