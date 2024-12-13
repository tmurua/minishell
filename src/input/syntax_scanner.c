/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_scanner.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 00:54:03 by dlemaire          #+#    #+#             */
/*   Updated: 2024/12/13 16:41:46 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_syntax_error(char *input, t_minishell *shell)
{
	if (!validate_heredoc_delimiter(input) || !check_missing_left(input)
		|| !check_missing_right(input) || !check_missing_middle(input))
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
int	check_missing_left(const char *str)
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

int	check_missing_right(const char *str)
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

int	check_missing_middle(const char *str)
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
