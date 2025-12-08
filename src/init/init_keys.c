#include "../inc/cub3d.h"

t_keys	*init_keys(void)
{
	t_keys	*k;

	k = malloc(sizeof(t_keys));
	if (!k)
		return (NULL);
	k->w = 0;
	k->a = 0;
	k->s = 0;
	k->d = 0;
	k->left = 0;
	k->right = 0;
	k->minimap = 0;
	k->fps = 0;
	k->e = 0;
	k->mouse_lock = 1;
	return (k);
}
