/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_checker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirifern <mirifern@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 21:42:18 by mirifern          #+#    #+#             */
/*   Updated: 2024/10/16 21:42:20 by mirifern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_tokens(t_data *d, char *input, int add_tokens, int pos)
{
	t_tokens	**new_tokens;
	char		**split_tokens;
	int			i;
	int			j;
	int			k;

	i = 0;
	j = 0;
	k = 0;
	split_tokens = ft_split(input, ' ');
	new_tokens = (t_tokens **)malloc(sizeof(t_tokens *)
			* ((d->tokens_qt + add_tokens) + 1));
	if (!new_tokens)
		ft_malloc(d, NULL, NULL);
	//inserto los rokens hasta encontrar el expandido
	while (i < pos)
	{
		new_tokens[j] = (t_tokens *)malloc(sizeof(t_tokens));
		if (!new_tokens[j])
			ft_malloc(d, NULL, NULL);
		new_tokens[j]->value = ft_strdup(d->tokens[i]->value);
		new_tokens[j]->type = d->tokens[i]->type;
		i++;
		j++;
	}
	// Insertar los nuevos tokens expandidos
	while (split_tokens[k])
	{
		new_tokens[j] = (t_tokens *)malloc(sizeof(t_tokens));
		if (!new_tokens[j])
			ft_malloc(d, NULL, NULL);
		new_tokens[j]->value = ft_strdup(split_tokens[k]);
		new_tokens[j]->type = CMD;
		k++;
		j++;
	}
	// Copiar el resto de los tokens después de 'pos'
	i = pos + 1;
	while (i < d->tokens_qt)
	{
		new_tokens[j] = (t_tokens *)malloc(sizeof(t_tokens));
		if (!new_tokens[j])
			ft_malloc(d, NULL, NULL);
		new_tokens[j]->value = ft_strdup(d->tokens[i]->value);
		new_tokens[j]->type = d->tokens[i]->type;
		i++;
		j++;
	}
	// Asegurarse de que el nuevo array termine con NULL
	new_tokens[j] = NULL;
	// Liberar el array original de tokens
	ft_free(d->tokens);
	// Actualizar el array de tokens y la cantidad de tokens en la estructura
	d->tokens = new_tokens;
	d->tokens_qt = d->tokens_qt + add_tokens;
	free_array(split_tokens);
}

/*Funcion que checkea si depues de expandir hay algun espacio dentro del token*/
void	check_var_spaces(t_data *data, char *input, int pos)
{
	int	i;
	int	add_tokens;

	i = 0;
	add_tokens = 0;
	(void) data;
	while (input[i] != '\0')
	{
		if (ft_isspace(input[i]))
			add_tokens++;
		i++;
	}
	if (add_tokens != 0)
		ft_add_tokens(data, input, add_tokens, pos);
}
