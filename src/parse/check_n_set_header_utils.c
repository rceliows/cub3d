#include "../inc/cub3d.h"

int	handle_duplicate_color(int *flag, char type)
{
	if (*flag)
	{
		if (type == 'F')
			ft_putstr_fd("Error: duplicate floor color\n", 2);
		else
			ft_putstr_fd("Error: duplicate ceiling color\n", 2);
		return (1);
	}
	return (0);
}

int	handle_empty_color(char *trimmed, char type)
{
	if (!trimmed || !*trimmed)
	{
		if (type == 'F')
			ft_putstr_fd("Error: floor color has no value\n", 2);
		else
			ft_putstr_fd("Error: ceiling color has no value\n", 2);
		return (1);
	}
	return (0);
}

int	handle_invalid_color(int rgb[3], char *trimmed, char type)
{
	if (!parse_rgb(trimmed, rgb))
	{
		if (type == 'F')
			ft_putstr_fd("Error: invalid floor color format\n", 2);
		else
			ft_putstr_fd("Error: invalid ceiling color format\n", 2);
		return (1);
	}
	return (0);
}

int	process_color_value(char *path, int rgb[3], char type)
{
	char	*trimmed;

	trimmed = skip_ws(path);
	if (handle_empty_color(trimmed, type))
		return (-1);
	if (handle_invalid_color(rgb, trimmed, type))
		return (-1);
	return (1);
}

char	*skip_ws(char *s)
{
	while (*s == ' ' || *s == '\t')
		s++;
	return (s);
}
