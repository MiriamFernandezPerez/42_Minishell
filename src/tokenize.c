/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirifern <mirifern@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:09:23 by mirifern          #+#    #+#             */
/*   Updated: 2024/07/19 00:09:17 by mirifern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tok_delimiter(t_data *data, char *input, int *ind, int *start)
{
	data->tokens[*ind] = malloc(sizeof(t_tokens));
	if (!data->tokens[*ind])
		exit(EXIT_FAILURE);
	data->tokens[*ind]->value = malloc(sizeof(char) * 2);
	if (!data->tokens[*ind]->value)
	{
		free(data->tokens[*ind]);
		exit(EXIT_FAILURE);
	}
	data->tokens[*ind]->value[0] = input[*start];
	data->tokens[*ind]->value[1] = '\0';
	data->tokens[*ind]->type = ft_isdelimiter(input[*start]);
	(*ind)++;
	(*start)++;
}

char	*tok_nodelimiter(t_data *data, int *ind, int *start, int *end)
{
	char	*str;

	str = malloc((*end - *start + 1) * sizeof(char));
	if (!str)
		exit(EXIT_FAILURE);
	ft_strlcpy(str, data->prompt + *start, *end - *start + 1);
	data->tokens[*ind] = malloc(sizeof(t_tokens));
	if (!data->tokens[*ind])
	{
		free(str);
		exit(EXIT_FAILURE);
	}
	data->tokens[*ind]->value = str;
	if (ft_isdelimiter(data->prompt[*start]) == SQUOTE
		|| ft_isdelimiter(data->prompt[*start]) == DQUOTE)
		data->tokens[*ind]->type = ARG;
	else if (ft_isdelimiter(data->prompt[*start]) == INPUT)
		data->tokens[*ind]->type = 7;
	else if (ft_isdelimiter(data->prompt[*start]) == TRUNC)
		data->tokens[*ind]->type = 8;
	else
		data->tokens[*ind]->type = ARG;
	(*ind)++;
	*start = *end;
	return (str);
}

void	define_delimiter(t_data *data, int *start, int *end, int *index)
{
	if (ft_isdelimiter(data->prompt[*start]) == SQUOTE
		|| ft_isdelimiter(data->prompt[*start]) == DQUOTE)
	{
		*end = end_quote(data->prompt, data->prompt[*start], *start + 1) + 1;
		tok_nodelimiter(data, index, start, end);
	}
	else if (ft_isdelimiter(data->prompt[*start]) == INPUT
		&& ft_isdelimiter(data->prompt[*start + 1]) == INPUT)
	{
		*end = end_quote(data->prompt, data->prompt[*start], *start + 1) + 1;
		tok_nodelimiter(data, index, start, end);
	}
	else if (ft_isdelimiter(data->prompt[*start]) == TRUNC
		&& ft_isdelimiter(data->prompt[*start + 1]) == TRUNC)
	{
		*end = end_quote(data->prompt, data->prompt[*start], *start + 1) + 1;
		tok_nodelimiter(data, index, start, end);
	}
	else if (ft_isdelimiter(data->prompt[*start]) == SPACES
		&& ft_isdelimiter(data->prompt[(*start) + 1]) == SPACES)
		(*start)++;
	else
		tok_delimiter(data, data->prompt, index, start);
}

void	ft_move_tokens(t_data *data, int *i, int *j)
{
	while (*j < data->tokens_qt - 1)
	{
		data->tokens[*j] = data->tokens[*j + 1];
		(*j)++;
	}
	data->tokens_qt--;
	data->tokens[data->tokens_qt] = NULL;
	(*i)--;
}

void	join_tokens(t_data *d, int i, int j)
{
	char	*new_value;

	while (i < d->tokens_qt - 1)
	{
		if (d->tokens[i]->type == ARG && d->tokens[i + 1]->type == ARG)
		{
			new_value = malloc(strlen(d->tokens[i]->value)
					+ strlen(d->tokens[i + 1]->value) + 1);
			if (!new_value)
				exit(EXIT_FAILURE);
			ft_strlcpy(new_value, d->tokens[i]->value,
				ft_strlen(d->tokens[i]->value) + 1);
			ft_strlcat(new_value, d->tokens[i + 1]->value,
				(ft_strlen(new_value)
					+ ft_strlen(d->tokens[i + 1]->value)) + 1);
			free(d->tokens[i]->value);
			d->tokens[i]->value = new_value;
			free(d->tokens[i + 1]->value);
			free(d->tokens[i + 1]);
			j = i + 1;
			ft_move_tokens(d, &i, &j);
		}
		i++;
	}
}

void	ft_tokenizer(t_data *d, int len, int start, int index)
{
	int	end;

	d->tokens = malloc(MAX_TOKENS * sizeof(t_tokens *));
	if (!d->tokens)
		exit(EXIT_FAILURE);
	while (start < len)
	{
		end = start;
		if (ft_isdelimiter(d->prompt[start]))
			define_delimiter(d, &start, &end, &index);
		else
		{
			while (end < len && !ft_isdelimiter(d->prompt[end]))
				end++;
			tok_nodelimiter(d, &index, &start, &end);
			if (index - 1 == 0)
				d->tokens[0]->type = CMD;
		}
	}
	d->tokens[index] = NULL;
	d->tokens_qt = index;
	join_tokens(d, 0, 0);
}