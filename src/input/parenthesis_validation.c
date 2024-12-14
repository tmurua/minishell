/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_validation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 18:19:03 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/14 18:55:51 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	validate_balanced_parenthesis(const char *str)
{
	int		counter;
	int		in_quotes;
	char	quote_char;

	counter = 0;
	in_quotes = 0;
	quote_char = '\0';
	while (*str)
	{
		if (handle_quotes((char *)str, &in_quotes, &quote_char))
			str++;
		else if (in_quotes)
			str++;
		else
		{
			if (*str == '(')
				counter++;
			else if (*str == ')')
				counter--;
			str++;
		}
	}
	return (counter == 0);
}

int	validate_opening_parenthesis(const char *str)
{
	char		prev_char;
	int			in_quotes;
	char		quote_char;
	const char	*new_str;

	prev_char = '\0';
	in_quotes = 0;
	quote_char = '\0';
	str = skip_whitespace_input((char *)str);
	while (*str)
	{
		new_str = process_opening_char(str, &in_quotes, &quote_char,
				&prev_char);
		if (!new_str)
			return (0);
		str = new_str;
	}
	return (1);
}

int	validate_closing_parenthesis(const char *str)
{
	int		in_quotes;
	char	quote_char;

	in_quotes = 0;
	quote_char = '\0';
	while (*str)
	{
		if (handle_quotes((char *)str, &in_quotes, &quote_char))
			str++;
		else if (in_quotes)
			str++;
		else
		{
			while (ft_iswhitespace(*str))
				str++;
			if (*str == '\0')
				break ;
			if (*str == ')' && !check_closing(&str))
				return (0);
			else if (*str != ')')
				str++;
		}
	}
	return (1);
}

int	validate_no_empty_parenthesis(const char *str)
{
	int		in_quotes;
	char	quote_char;

	in_quotes = 0;
	quote_char = '\0';
	while (*str)
	{
		if (handle_quotes((char *)str, &in_quotes, &quote_char))
			str++;
		else if (in_quotes)
			str++;
		else
		{
			while (ft_iswhitespace(*str))
				str++;
			if (*str == '\0')
				break ;
			if (*str == '(' && !check_empty_parenthesis(&str))
				return (0);
			else if (*str != '(')
				str++;
		}
	}
	return (1);
}
