#include "../inc/cub3d.h"

void	rstrip_nl(char *s)
{
	int	i;

	if (!s)
		return ;
	i = (int)ft_strlen(s);
	while (i > 0 && (s[i - 1] == '\n' || s[i - 1] == '\r'))
	{
		s[i - 1] = '\0';
		i--;
	}
}

void	ft_free_split(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
