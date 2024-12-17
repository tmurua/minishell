/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_validation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 18:19:03 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/17 05:01:50 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	update_quote_states(char c, int *in_single_quote, int *in_double_quote);

int	validate_balanced_parenthesis(const char *str)
{
	int	counter;
	int	in_single_quote;
	int	in_double_quote;

	counter = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (*str)
	{
		if (*str == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*str == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (*str == '(' && !in_single_quote && !in_double_quote)
			counter++;
		else if (*str == ')' && !in_single_quote && !in_double_quote)
			counter--;
		str++;
	}
	return (counter == 0);
}

int	validate_opening_parenthesis(const char *str)
{
	int		in_single_quote;
	int		in_double_quote;
	char	prev_char;
	int		i;

	in_single_quote = 0;
	in_double_quote = 0;
	prev_char = '\0';
	i = 0;
	while (str[i] && *(str + 1) != '\0')
	{
		while (ft_iswhitespace(str[i]))
			i++;
		update_quote_states(str[i], &in_single_quote, &in_double_quote);
		if (str[i] == '(' && !in_single_quote && !in_double_quote)
		{
			if (prev_char != '\0' && prev_char != '|' && prev_char != '&'
				&& prev_char != '(' && !ft_iswhitespace(prev_char))
				return (0);
		}
		if (!in_single_quote && !in_double_quote)
			prev_char = str[i];
		i++;
	}
	return (1);
}

int	validate_closing_parenthesis(const char *str)
{
	int	i;
	int	in_single_quote;
	int	in_double_quote;

	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i])
	{
		update_quote_states(str[i], &in_single_quote, &in_double_quote);
		if (str[i] == ')' && !in_single_quote && !in_double_quote)
		{
			i++;
			while (str[i] && ft_iswhitespace(str[i]))
				i++;
			if (str[i] != '\0' && str[i] != '|' && str[i] != '&'
				&& str[i] != ')')
				return (0);
		}
		i++;
	}
	return (1);
}

void	update_quote_states(char c, int *in_single_quote, int *in_double_quote)
{
	if (c == '\'' && !*in_double_quote)
		*in_single_quote = !*in_single_quote;
	else if (c == '\"' && !*in_single_quote)
		*in_double_quote = !*in_double_quote;
}

int	validate_no_empty_parenthesis(const char *str)
{
	while (*str && *(str + 1) != '\0')
	{
		while (ft_iswhitespace(*str))
			str++;
		if (*str == '(')
		{
			str++;
			while (ft_iswhitespace(*str))
				str++;
			if (*str == ')')
			{
				str++;
				while (ft_iswhitespace(*str))
					str++;
				if (*str == '\0')
					return (0);
			}
		}
		else
			str++;
	}
	return (1);
}
