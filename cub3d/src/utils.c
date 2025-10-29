/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 15:09:04 by rceliows          #+#    #+#             */
/*   Updated: 2025/10/28 15:09:05 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	cleanup_cub3d(t_cub3d *cub3d)
{
	cleanup_window(cub3d->window);
//	cleanup_map(cub3d->map);
}

void	error_exit(t_cub3d *cub3d)
{
	cleanup_cub3d(cub3d);
	exit(1);
}
