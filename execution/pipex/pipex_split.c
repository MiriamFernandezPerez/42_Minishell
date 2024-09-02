/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:50:28 by esellier          #+#    #+#             */
/*   Updated: 2024/05/27 20:22:54 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

int	count_path(char *str, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == c && str[i + 1] != c)
			count++;
		i++;
	}
	return (count);
}

int	strlen_path(char *str, char c)
{
	int	len;

	len = 0;
	while (str[len] != c && str[len] != '\0')
		len++;
	return (len);
}

void	free_array(char **array)
{
	int	j;

	j = 0;
	while (array[j])
	{
		free (array[j]);
		j++;
	}
	if (array)
		free (array);
}

char	**put_path(char **array, char *str, char c, int path)
{
	int	i;
	int	j;
	int	len;
	int	s;

	j = 0;
	s = 0;
	while (j < path)
	{
		i = 0;
		len = strlen_path(&str[s], c);
		array[j] = (char *)malloc((len + 1) * sizeof (char));
		if (!array[j])
			free_array(array);
		while (str[s] != c)
			array[j][i++] = str[s++];
		array[j][i] = '\0';
		j++;
		s++;
	}
	if (j == path)
		array[j] = NULL;
	return (array);
}

char	**split_env(char *str)
{
	char	**array;
	int		path;
	char	c;

	c = ':';
	if (! str)
		return (NULL);
	path = count_path(str, c);
	array = (char **)malloc((path + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	return (put_path(array, str, c, path));
}