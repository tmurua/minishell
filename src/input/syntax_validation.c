/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:29:20 by dlemaire          #+#    #+#             */
/*   Updated: 2024/12/17 05:00:42 by tmurua           ###   ########.fr       */
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
	int		in_quotes;
	char	quote_char;

	in_quotes = 0;
	quote_char = '\0';
	while (*str)
	{
		if (handle_quotes(str, &in_quotes, &quote_char))
			str++;
		else if (in_quotes)
			str++;
		else if (ft_strncmp(str, "<<", 2) == 0 && !check_heredoc_char(&str))
			return (0);
		else
			str++;
	}
	return (1);
}

int	validate_delimiter_left(const char *str)
{
	int		in_quotes;
	char	quote_char;

	in_quotes = 0;
	quote_char = '\0';
	str = skip_whitespace_input((char *)str);
	while (*str)
	{
		if (handle_quotes((char *)str, &in_quotes, &quote_char))
			str++;
		else if (in_quotes)
			str++;
		else
			return (check_left_char(str));
		str++;
	}
	return (1);
}

int	validate_delimiter_right(const char *str)
{
	const char	*last_valid = NULL;

	while (*str && *(str + 1) != '\0')
	{
		str = skip_whitespace_input((char *)str);
		if (*str == '|' || (*str == '&' && *(str + 1) == '&'))
		{
			last_valid = str;
			if (*str == '|')
				str++;
			else
				str += 2;
		}
		else
		{
			str++;
		}
	}
	if (last_valid)
	{
		str = skip_whitespace_input((char *)(last_valid + 1));
		if (*str == '\0')
			return (0);
	}
	return (1);
}

// int	validate_delimiter_right(const char *str)
// {
// 	const char	*last_valid = NULL;
// 	int			offset;
// 	int			in_quotes;
// 	char		quote_char;

// 	offset = 1;
// 	in_quotes = 0;
// 	quote_char = '\0';
// 	while (*str)
// 	{
// 		str = skip_whitespace_input((char *)str);
// 		if (handle_quotes((char *)str, &in_quotes, &quote_char))
// 			str++;
// 		else if (in_quotes)
// 			str++;
// 		else
// 			str = skip_operator_right(str, &last_valid, &offset);
// 	}
// 	return (check_right_end(last_valid, offset));
// }

int	validate_delimiter_middle(const char *str)
{
	int			in_quotes;
	char		quote_char;
	const char	*new_str;

	in_quotes = 0;
	quote_char = '\0';
	while (*str && *(str + 1) != '\0')
	{
		new_str = process_delimiter_char(str, &in_quotes, &quote_char);
		if (!new_str)
			return (0);
		str = new_str;
	}
	return (1);
}
