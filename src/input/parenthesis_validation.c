/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_validation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 18:19:03 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/15 18:15:28 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	int		i;
	char	prev_char;

	in_single_quote = 0;
	in_double_quote = 0;
	i = 0;
	prev_char = '\0';
	while (str[i])
	{
		while (str[i] && ft_iswhitespace(str[i]))
			i++;
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (str[i] == '(' && !in_single_quote && !in_double_quote)
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
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (str[i] == ')' && !in_single_quote && !in_double_quote)
		{
			i++;
			while (str[i] && ft_iswhitespace(str[i]))
				i++;
			if (str[i] != '\0' && str[i] != '|' && str[i] != '&'
				&& str[i] != ')')
				return (0);
			continue ;
		}
		i++;
	}
	return (1);
}

int	validate_no_empty_parenthesis(const char *str)
{
	while (*str)
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


// int	validate_balanced_parenthesis(const char *str)
// {
// 	int		counter;
// 	int		in_quotes;
// 	char	quote_char;

// 	counter = 0;
// 	in_quotes = 0;
// 	quote_char = '\0';
// 	while (*str)
// 	{
// 		if (handle_quotes((char *)str, &in_quotes, &quote_char))
// 			str++;
// 		else if (in_quotes)
// 			str++;
// 		else
// 		{
// 			if (*str == '(')
// 				counter++;
// 			else if (*str == ')')
// 				counter--;
// 			str++;
// 		}
// 	}
// 	return (counter == 0);
// }

// int	validate_opening_parenthesis(const char *str)
// {
// 	char		prev_char;
// 	int			in_quotes;
// 	char		quote_char;
// 	const char	*new_str;

// 	prev_char = '\0';
// 	in_quotes = 0;
// 	quote_char = '\0';
// 	str = skip_whitespace_input((char *)str);
// 	while (*str)
// 	{
// 		new_str = process_opening_char(str, &in_quotes, &quote_char,
// 				&prev_char);
// 		if (!new_str)
// 			return (0);
// 		str = new_str;
// 	}
// 	return (1);
// }

// int	validate_closing_parenthesis(const char *str)
// {
// 	int		in_quotes;
// 	char	quote_char;

// 	in_quotes = 0;
// 	quote_char = '\0';
// 	while (*str)
// 	{
// 		if (handle_quotes((char *)str, &in_quotes, &quote_char))
// 			str++;
// 		else if (in_quotes)
// 			str++;
// 		else
// 		{
// 			while (ft_iswhitespace(*str))
// 				str++;
// 			if (*str == '\0')
// 				break ;
// 			if (*str == ')' && !check_closing(&str))
// 				return (0);
// 			else if (*str != ')')
// 				str++;
// 		}
// 	}
// 	return (1);
// }


// int	validate_no_empty_parenthesis(const char *str)
// {
// 	int		in_quotes;
// 	char	quote_char;

// 	in_quotes = 0;
// 	quote_char = '\0';
// 	while (*str)
// 	{
// 		if (handle_quotes((char *)str, &in_quotes, &quote_char))
// 			str++;
// 		else if (in_quotes)
// 			str++;
// 		else
// 		{
// 			while (ft_iswhitespace(*str))
// 				str++;
// 			if (*str == '\0')
// 				break ;
// 			if (*str == '(' && !check_empty_parenthesis(&str))
// 				return (0);
// 			else if (*str != '(')
// 				str++;
// 		}
// 	}
// 	return (1);
// }
