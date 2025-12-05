/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_window.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceliows <rceliows@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 16:37:57 by rceliows          #+#    #+#             */
/*   Updated: 2025/10/29 16:37:59 by rceliows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

t_window	*init_window(void)
{
	t_window	*w;

	w = malloc(sizeof(t_window));
	if (!w)
		return (NULL);
	w->mlx = mlx_init();
	if (!w->mlx)
		return (NULL);
	w->win = mlx_new_window(w->mlx, DEFSCREENWIDTH, DEFSCREENHEIGHT, "cub3d");
	if (!w->win)
		return (NULL);
	w->img = mlx_new_image(w->mlx, DEFSCREENWIDTH, DEFSCREENHEIGHT);
	if (!w->img)
		return (NULL);
	w->center_x = DEFSCREENWIDTH / 2;
	w->center_y = DEFSCREENHEIGHT / 2;
	return (w);
}

void	cleanup_window(t_window *w)
{
	if (w->img)
	{
		mlx_destroy_image(w->mlx, w->img);
		w->img = NULL;
	}
	if (w->win)
	{
		mlx_destroy_window(w->mlx, w->win);
		w->win = NULL;
	}
	if (w->mlx)
	{
		mlx_destroy_display(w->mlx);
		free(w->mlx);
		w->mlx = NULL;
	}
}
