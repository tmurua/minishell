/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_scanner.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 00:54:03 by dlemaire          #+#    #+#             */
/*   Updated: 2024/12/14 02:17:29 by dlemaire         ###   ########.fr       */
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
			while (ft_iswhitespace(*(str + 2)))
				str++;
			if (ft_strncmp(str + 2, "<", 1) == 0
				|| ft_strncmp(str + 2, ">", 1) == 0
				|| ft_strncmp(str + 2, "(", 1) == 0
				|| ft_strncmp(str + 2, ")", 1) == 0
				|| ft_strncmp(str + 2, "&", 1) == 0
				|| ft_strncmp(str + 2, "#", 1) == 0
				|| ft_strncmp(str + 2, "-", 1) == 0
				|| ft_strncmp(str + 2, "|", 1) == 0
				|| ft_strncmp(str + 2, "", 1) == 0)
				return (0);
		}
		str++;
	}
	return (1);
}

char	*skip_whitespace_input(char *str)
{
	while (*str && ft_iswhitespace(*str))
		str++;
	return (str);
}

int	validate_delimiter_left(const char *str)
{
	str = skip_whitespace_input((char *)str);
	if (*str == '|' || (*str == '&' && *(str + 1) == '&'))
		return (0);
	return (1);
}

int	validate_delimiter_right(const char *str)
{
	const char	*last_valid;
	int			offset;

	last_valid = NULL;
	offset = 1;
	while (*str)
	{
		str = skip_whitespace_input((char *)str);
		if (*str == '|' || (*str == '&' && *(str + 1) == '&'))
		{
			last_valid = str;
			if (*str == '|')
				str++;
			else
			{
				str += 2;
				offset = 2;
			}
		}
		else
			str++;
	}
	if (last_valid)
	{
		str = skip_whitespace_input((char *)(last_valid + offset));
		if (*str == '\0')
			return (0);
	}
	return (1);
}

int	validate_delimiter_middle(const char *str)
{
	while (*str)
	{
		str = skip_whitespace_input((char *)str);
		if (*str == '|')
		{
			if (*(str + 1) == '|')
				str += 2;
			else
				str++;
			str = skip_whitespace_input((char *)str);
			if (*str == '|' || *str == '&')
				return (0);
		}
		else if (*str == '&' && *(str + 1) == '&')
		{
			str += 2;
			str = skip_whitespace_input((char *)str);
			if (*str == '|' || *str == '&')
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

	counter = 0;
	while (*str)
	{
		if (*str == '(')
			counter++;
		else if (*str == ')')
			counter--;
		str++;
	}
	if (counter == 0)
		return (1);
	else
		return (0);
}

int	validate_opening_parenthesis(const char *str)
{
	char	prev_char;
	int		i;

	prev_char = '\0';
	i = 0;
	while (str[i])
	{
		while (ft_iswhitespace(str[i]))
			i++;
		if (str[i] == '(')
		{
			if (prev_char != '\0' && prev_char != '|' && prev_char != '&'
				&& prev_char != '(')
				return (0);
		}
		prev_char = str[i];
		i++;
	}
	return (1);
}

int	validate_closing_parenthesis(const char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		while (ft_iswhitespace(str[i]))
			i++;
		if (str[i] == ')')
		{
			i++;
			while (ft_iswhitespace(str[i]))
				i++;
			if (str[i] != '\0' && str[i] != '|' && str[i] != '&'
				&& str[i] != ')')
				return (0);
		}
		else
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
