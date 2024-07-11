/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirifern <mirifern@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:09:23 by mirifern          #+#    #+#             */
/*   Updated: 2024/07/10 18:01:58 by mirifern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tok_delimiter(t_data **data, char *input, int *ind, int *start)
{
	(*data)->tokens[*ind] = malloc(sizeof(t_tokens));
	if (!(*data)->tokens[*ind])
		exit(EXIT_FAILURE);
	(*data)->tokens[*ind]->value = malloc(sizeof(char) * 2);
	if (!(*data)->tokens[*ind]->value)
		exit(EXIT_FAILURE);
	(*data)->tokens[*ind]->value[0] = input[*start];
	(*data)->tokens[*ind]->value[1] = '\0';
	(*data)->tokens[*ind]->type = ft_isdelimiter(input[*start]);
	(*ind)++;
	(*start)++;
}

char	*tok_nodelimiter(t_data **data, int *ind, int *start, int end)
{
	char	*input;
	char	*str;

	input = (*data)->prompt;
	str = malloc((end - *start + 1) * sizeof(char));
	if (!str)
		exit(EXIT_FAILURE);
	ft_strlcpy(str, input + *start, end - *start + 1);
	(*data)->tokens[*ind] = malloc(sizeof(t_tokens));
	if (!(*data)->tokens[*ind])
		exit(EXIT_FAILURE);
	(*data)->tokens[*ind]->value = str;
	(*data)->tokens[*ind]->type = ARG;
	(*ind)++;
	*start = end;
	return (str);
}

void	ft_tokenizer(t_data **d, int len, int start, int index)
{
	int	end;

	(*d)->tokens = malloc(MAX_TOKENS * sizeof(t_tokens *));
	if (!(*d)->tokens)
		exit(EXIT_FAILURE);
	while (start < len)
	{
		end = start;
		if (ft_isdelimiter((*d)->prompt[start]) == 5
			|| ft_isdelimiter((*d)->prompt[start]) == 6)
		{
			end = end_quote((*d)->prompt, (*d)->prompt[start], start + 1) + 1;
			tok_nodelimiter(d, &index, &start, end);
		}
		else if (ft_isdelimiter((*d)->prompt[start]))
			tok_delimiter(d, (*d)->prompt, &index, &start);
		else
		{
			while (end < len && !ft_isdelimiter((*d)->prompt[end]))
				end++;
			tok_nodelimiter(d, &index, &start, end);
		}
	}
	(*d)->tokens[index] = NULL;
	(*d)->tokens_qt = index;
}

/*busca que haya comillas bien cerradas y solo un pipe fuera de comillas 
y que no este al principio o al final de input*/
int	ft_parser(char *input)
{
	if (find_quotes(input, 0, 0) == 1)
		return (1);
	if (find_pipes(input) < 0)
		return (1);
	return (0);
}

/*primero hace un parseo del input para descartar posibles errores*/
int	parse_and_token(t_data **data, char *input)
{
	int		len;

	if (!input)
		exit(EXIT_FAILURE);
	len = ft_strlen(input);
	if (ft_parser(input) == 1)
		return (1);
	ft_tokenizer(data, len, 0, 0);
	print_tokens(data);
	return (0);
}
