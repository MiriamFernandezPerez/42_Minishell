/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_prompt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirifern <mirifern@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 18:44:22 by mirifern          #+#    #+#             */
/*   Updated: 2024/08/13 21:34:24 by mirifern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_other_chars(char *input)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '\\' || input[i] == ';')
		{
			ft_msn(ERR_OTHER, 2);
			return (1);
		}
		i++;
	}
	return (0);
}

/*Funcion que verifica si el string esta compuesto
solo por espacios y/o tabulaciones*/
int	only_spaces(char *s)
{
	int	i;
	int	check;

	i = 0;
	check = 0;
	while (s[i] != '\0')
	{
		if (ft_isspace(s[i]))
			check++;
		i++;
	}
	if (i == check)
		return (1);
	return (0);
}

void	token_expand_clean(t_data *data)
{
	ft_tokenizer(data, ft_strlen(data->prompt), 0, 0);
	ft_expander(data);
	print_tokens(data);
	delete_token_type(data, END);
	join_tokens(data, 0, 0);
	delete_token_type(data, SPACES);
}

/*1 - lee el input del prompt
2 - Si detecta un EOF (ctr+d) muestra un exit y devuelve -1 para que
se salga del bucle while(1) del main y cerrar el shell liberando
el input
3 - Anade el input al history
4 - Paso el input para depurarlo a la funcion ft_parser
5 - Tokeniza el input
...
*/
int	ft_read_prompt(t_data *data)
{
	data->prompt = readline("\033[1;34m👯 minishell> \033[0m");
	if (!data->prompt)
	{
		ft_msn(EXIT, 2);
		return (-1);
	}
	if (*data->prompt)
		add_history(data->prompt);
	if (ft_parser(data) == 1)
	{
		free(data->prompt);
		data->prompt = NULL;
		return (0);
	}
	token_expand_clean(data);
	ft_execute(data);
	free(data->prompt);
	data->prompt = NULL;
	ft_free(data->tokens);
	data->tokens = NULL;
	return (0);
}
