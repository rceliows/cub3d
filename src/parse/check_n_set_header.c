#include "../inc/cub3d.h"

static char	*dup_trim_right(const char *p)
{
	int		len;
	char	*s;
	int		start;
	int		end;

	if (!p)
		return (NULL);
	len = ft_strlen(p);
	start = 0;
	while (start < len && (p[start] == ' ' || p[start] == '\t'))
		start++;
	end = len;
	while (end > start && (p[end - 1] == ' ' || p[end - 1] == '\t' || p[end
				- 1] == '\n' || p[end - 1] == '\r'))
		end--;
	s = ft_substr(p, start, end - start);
	return (s);
}

static int	set_texture(char *path, char **texture, int *hdr_count, char *name)
{
	if (*texture)
	{
		ft_putstr_fd("Error: duplicate ", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd(" texture\n", 2);
		return (-1);
	}
	*texture = dup_trim_right(path);
	if (!*texture)
		return (-1);
	(*hdr_count)++;
	return (1);
}

static int	set_color(t_color_ctx ctx)
{
	int	result;

	if (handle_duplicate_color(ctx.flag, ctx.type))
		return (-1);
	result = process_color_value(ctx.path, ctx.rgb, ctx.type);
	if (result == -1)
		return (-1);
	(*ctx.flag) = 1;
	(*ctx.hdr_count)++;
	return (1);
}

int	check_n_set_header(char *line, t_data *g, int *hdr_count)
{
	char			*p;
	t_color_ctx		ctx;

	p = skip_ws(line);
	if (!ft_strncmp(p, "NO ", 3))
		return (set_texture(p + 3, &g->north_texture, hdr_count, "NO"));
	if (!ft_strncmp(p, "SO ", 3))
		return (set_texture(p + 3, &g->south_texture, hdr_count, "SO"));
	if (!ft_strncmp(p, "EA ", 3))
		return (set_texture(p + 3, &g->east_texture, hdr_count, "EA"));
	if (!ft_strncmp(p, "WE ", 3))
		return (set_texture(p + 3, &g->west_texture, hdr_count, "WE"));
	if (!ft_strncmp(p, "F ", 2))
	{
		ctx = (t_color_ctx){p + 2, g->floor, hdr_count, &g->have_floor, 'F'};
		return (set_color(ctx));
	}
	if (!ft_strncmp(p, "C ", 2))
	{
		ctx = (t_color_ctx){p + 2, g->ceiling,
			hdr_count, &g->have_ceiling, 'C'};
		return (set_color(ctx));
	}
	return (0);
}
