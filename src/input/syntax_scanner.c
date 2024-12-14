/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_scanner.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/12/14 18:49:17 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

int	handle_syntax_error(char *input, t_minishell *shell)
{
	if (!validate_heredoc_delimiter(input) || !validate_delimiter_left(input)
		|| !validate_delimiter_right(input) || !validate_delimiter_middle(input)
		|| !validate_balanced_parenthesis(input)
		|| !validate_opening_parenthesis(input)
		|| !validate_closing_parenthesis(input)
		|| !validate_no_empty_parenthesis(input))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		shell->last_exit_status = 2;
		add_history(input);
		free(input);
		return (1);
	}
	return (0);
}

int	validate_heredoc_delimiter(char *str)
{
	while (*str)
	{
		if (ft_strncmp(str, "<<", 2) == 0)
		{
			str += 2;
			while (*str && ft_iswhitespace(*str))
				str++;
			if (!*str)
				return (0);
			if (*str == '<' || *str == '>' || *str == '(' || *str == ')'
				|| *str == '&' || *str == '#' || *str == '-' || *str == '|'
				|| *str == '\0')
				return (0);
		}
		str++;
	}
	return (1);
}

/* check if first non-whitespace char indicates a missing token on left */
int	validate_delimiter_left(const char *str)
{
	str = skip_whitespace_input((char *)str);
	if (*str == '\0')
		return (1);
	if (*str == '|')
		return (0);
	if (*str == '&' && *(str + 1) != '\0' && *(str + 1) == '&')
		return (0);
	return (1);
}

int	validate_delimiter_right(const char *str)
{
	const char	*last_valid = NULL;
	int			offset;
	const char	*check_pos;

	offset = 1;
	while (*str)
	{
		str = skip_whitespace_input((char *)str);
		if (*str == '\0')
			break ;
		str = skip_operator_right(str, &last_valid, &offset);
	}
	if (last_valid)
	{
		check_pos = last_valid + offset;
		check_pos = skip_whitespace_input((char *)check_pos);
		if (*check_pos == '\0')
			return (0);
	}
	return (1);
}

int	validate_delimiter_middle(const char *str)
{
	while (*str)
	{
		str = skip_whitespace_input((char *)str);
		if (*str == '\0')
			break ;
		if (*str == '|')
		{
			str = handle_pipe_middle(str);
			if (str == NULL)
				return (0);
		}
		else if (*str == '&')
		{
			str = handle_and_middle(str);
			if (str == NULL)
				return (0);
		}
		else
			str++;
	}
	return (1);
}

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
	char	prev_char;
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	prev_char = '\0';
	i = 0;
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
			continue;
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
