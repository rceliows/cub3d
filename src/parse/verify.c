#include "../inc/cub3d.h"

int	has_xpm_extension(const char *path)
{
	size_t	len;

	if (!path)
		return (0);
	len = ft_strlen(path);
	if (len < 5)
		return (0);
	return (ft_strncmp(path + len - 4, ".xpm", 4) == 0);
}

int	is_valid_texture_path(const char *raw)
{
	int		fd;
	char	*path;

	if (!raw)
		return (-1);
	path = (char *)raw;
	while (*path == ' ' || *path == '\t')
		path++;
	if (!has_xpm_extension(path))
		return (0);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);
	close(fd);
	return (1);
}

static int	check_texture_file(const char *path, const char *name)
{
	int	result;

	result = is_valid_texture_path(path);
	if (result == 0)
	{
		ft_putstr_fd("Error: ", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd(" texture must be .xpm file\n", 2);
		return (1);
	}
	if (result == -1)
	{
		ft_putstr_fd("Error: ", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd(" texture", 2);
		perror("");
		return (2);
	}
	return (0);
}

static int	validate_all_texture_files(t_data *game)
{
	int	result;

	result = check_texture_file(game->north_texture, "NO");
	if (result != 0)
		return (result);
	result = check_texture_file(game->south_texture, "SO");
	if (result != 0)
		return (result);
	result = check_texture_file(game->west_texture, "WE");
	if (result != 0)
		return (result);
	result = check_texture_file(game->east_texture, "EA");
	if (result != 0)
		return (result);
	return (0);
}

int	validate_textures(t_data *game)
{
	int	result;

	if (!game->north_texture)
		return (ft_putstr_fd("Error: missing NO texture\n", 2), 1);
	if (!game->south_texture)
		return (ft_putstr_fd("Error: missing SO texture\n", 2), 1);
	if (!game->east_texture)
		return (ft_putstr_fd("Error: missing EA texture\n", 2), 1);
	if (!game->west_texture)
		return (ft_putstr_fd("Error: missing WE texture\n", 2), 1);
	result = validate_all_texture_files(game);
	if (result != 0)
		return (result);
	if (!game->have_floor)
		return (ft_putstr_fd("Error: missing floor color\n", 2), 1);
	if (!game->have_ceiling)
		return (ft_putstr_fd("Error: missing ceiling color\n", 2), 1);
	return (0);
}

int	verify_map(const char *path, t_data *game)
{
	int	fd;
	int	result;

	if (!has_cub_extension(path))
		return (ft_putstr_fd("Error: file must have .cub extension\n", 2), 1);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (perror("Error"), 2);
	close(fd);
	game->map = parse_map((char *)path, game);
	if (!game->map)
		return (1);
	if (!game->map[0])
		return (ft_putstr_fd("Error: map is empty\n", 2), 1);
	result = validate_textures(game);
	if (result != 0)
		return (result);
	if (!validate_map(game))
		return (1);
	return (0);
}
