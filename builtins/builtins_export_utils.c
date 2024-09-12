/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:17:55 by esellier          #+#    #+#             */
/*   Updated: 2024/09/05 20:30:57 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*check_value(t_data *data, char *value, int j, char *name)
{
	value = malloc((j + 1) * sizeof(char));
	if (!value)
	{
		free(name);
		ft_malloc(data, NULL, NULL);
	}
	return (value);
}

int	check_name(char *str, int i, t_env *current, t_data *data)
{
	char	*name;
	char	*value;
	int		j;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] != '=')
		return (0);
	name = malloc((i + 1) * sizeof(char));
	if (!name)
		ft_malloc(data, NULL, NULL);
	ft_strlcpy(name, str, i + 1);
	while (current && ft_strcmp(name, current->name) != 0)
		current = current->next;
	if (current && ft_strcmp(name, current->name) == 0 && current->value)
	{
		j = 0;
		while (str[i++])
			j++;
		value = NULL;
		value = check_value(data, value, j, name);
		ft_strlcpy(value, &str[i - j], j + 1);
		free(current->value);
		current->value = value;
		return (free(name), 1);
	}
	return (free(name), 0);
}

int	check_args(char *str)
{
	int	i;

	i = 0;
	if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')
		|| str[i] == '_')
		i++;
	else
	{
		write(2, "👯 minishell> : export: ", 26);
		write(2, str, ft_strlen(str));
		return (write(2, ": not a valid identifier\n", 25), 1);
	}
	while (str[i] && ((str[i] >= 'a' && str[i] <= 'z')
			|| (str[i] >= 'A' && str[i] <= 'Z') || str[i] == '_'
			|| str[i] == '=' || (str[i] >= '0' && str[i] <= '9')))
		if (str[i++] == '=')
			return (0);
	if (str[i])
	{
		write(2, "👯 minishell> : export: ", 26);
		write(2, str, ft_strlen(str));
		return (write(2, ": not a valid identifier\n", 25), 1);
	}
	return (0);
}

//checkname= check si la variable existe ou non,
//si exite pas, cree le node, sinon modifie la value de celui-ci