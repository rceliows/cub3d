/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 19:43:50 by rceliows          #+#    #+#             */
/*   Updated: 2025/12/05 19:43:54 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	has_cub_extension(const char *map)
{
	size_t	len;

	if (!map)
		return (0);
	len = ft_strlen(map);
	if (len < 5)
		return (0);
	return (ft_strncmp(map + len - 4, ".cub", 4) == 0);
}

int	ft_isnumber(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	parse_rgb(char *line, int rgb[3])
{
	char	**n_split;
	int		i;
	char	*trimmed;

	n_split = ft_split(line, ',');
	if (!n_split)
		return (0);
	i = 0;
	while (n_split[i])
		i++;
	if (i != 3)
		return (ft_free_split(n_split), 0);
	i = 0;
	while (i < 3)
	{
		trimmed = ft_strtrim(n_split[i], " \t");
		if (!trimmed || !ft_isnumber(trimmed))
			return (ft_free_split(n_split), free(trimmed), 0);
		rgb[i] = ft_atoi(trimmed);
		free(trimmed);
		if (rgb[i] < 0 || rgb[i] > 255)
			return (ft_free_split(n_split), 0);
		i++;
	}
	return (ft_free_split(n_split), 1);
}

int	is_blank(const char *s)
{
	int	i;

	i = 0;
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r')
		i++;
	return (s[i] == '\0');
}

int	check_if_valid(const char *s)
{
	int	i;
	int	seen;

	i = 0;
	seen = 0;
	while (s[i])
	{
		if (s[i] != '0' && s[i] != '1' && s[i] != 'N' && s[i] != 'S'
			&& s[i] != 'E' && s[i] != 'W' && s[i] != ' ' && s[i] != '\n'
			&& s[i] != '\t' && s[i] != 'D' && s[i] != 'P' && s[i] != 'A')
			return (0);
		if (s[i] != ' ' && s[i] != '\n' && s[i] != '\t')
			seen = 1;
		i++;
	}
	return (seen);
}
