/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_scanner.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 00:54:03 by dlemaire          #+#    #+#             */
/*   Updated: 2024/12/12 23:49:07 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_syntax_error(char *input)
{
	if (!validate_heredoc_delimiter(input) || !check_missing_left(input)
		|| !check_missing_right(input) || !check_missing_middle(input))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
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
				|| ft_strncmp(str + 2, "|", 1) == 0)
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

int	check_missing_left(const char *str)
{
	str = skip_whitespace_input((char *)str);
	if (*str == '|' || (*str == '&' && *(str + 1) == '&'))
		return (0);
	return (1);
}

int	check_missing_right(const char *str)
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

int	check_missing_middle(const char *str)
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


