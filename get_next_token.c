/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 16:42:16 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/07 19:22:12 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define TOKEN_COMPLETE 1
#define TOKEN_CONTINUE 0
#define TOKEN_ERROR   -1

/*	after skipping any leading whitespace, collect the next token,
	determine its type (BUILTIN_CMD or ARGUMENT) and return * to new token */
t_token	*get_next_token(t_lexer *lexer)
{
	t_token	*token;
	char	*value;

	skip_whitespace(lexer);
	if (lexer->current_char == '\0')
		return (NULL);
	value = collect_token(lexer);
	if (!value)
		return (NULL);
	if (is_builtin_command(value))
		token = create_token(BUILTIN_CMD, value);
	else
		token = create_token(ARGUMENT, value);
	return (token);
}

char	*collect_token(t_lexer *lexer)
{
	char	*buffer;
	int		status;

	buffer = ft_strdup("");
	if (!buffer)
		return (NULL);
	while (lexer->current_char != '\0')
	{
		if (lexer->state == DEFAULT_STATE)
			status = handle_default_state(lexer, &buffer);
		else if (lexer->state == SINGLE_QUOTE_STATE)
			status = handle_single_quote_state(lexer, &buffer);
		else if (lexer->state == DOUBLE_QUOTE_STATE)
			status = handle_double_quote_state(lexer, &buffer);
		if (status == TOKEN_COMPLETE)
			break ;
		else if (status == TOKEN_ERROR)
		{
			free(buffer);
			return (NULL);
		}
	}
	return (buffer);
}

int	handle_default_state(t_lexer *lexer, char **buffer)
{
	if (ft_iswhitespace(lexer->current_char))
		return (TOKEN_COMPLETE);
	else if (lexer->current_char == '\'')
	{
		lexer->state = SINGLE_QUOTE_STATE;
		move_forward(lexer);
	}
	else if (lexer->current_char == '"')
	{
		lexer->state = DOUBLE_QUOTE_STATE;
		move_forward(lexer);
	}
	/*else if (lexer->current_char == '$')
	{
		if (!handle_variable_expansion(lexer, buffer))
			return (TOKEN_ERROR);
	}*/
	else
	{
		if (!advance_and_append(lexer, buffer))
			return (TOKEN_ERROR);
	}
	return (TOKEN_CONTINUE);
}

int	handle_single_quote_state(t_lexer *lexer, char **buffer)
{
	if (lexer->current_char == '\'')
	{
		lexer->state = DEFAULT_STATE;
		move_forward(lexer);
	}
	else
	{
		if (!advance_and_append(lexer, buffer))
			return (TOKEN_ERROR);
	}
	return (TOKEN_CONTINUE);
}

int	handle_double_quote_state(t_lexer *lexer, char **buffer)
{
	if (lexer->current_char == '"')
	{
		lexer->state = DEFAULT_STATE;
		move_forward(lexer);
	}
	/*else if (lexer->current_char == '$')
	{
		if (!handle_variable_expansion(lexer, buffer))
			return (-1);
	}*/
	else
	{
		if (!advance_and_append(lexer, buffer))
			return (TOKEN_ERROR);
	}
	return (TOKEN_CONTINUE);
}

/*	current_char_str: temp str to hold current_char as a null-terminated str
	join current buffer with current_char_str at the end, move lexer forward */
int	advance_and_append(t_lexer *lexer, char **buffer)
{
	char	current_char_str[2];
	char	*new_buffer;

	current_char_str[0] = lexer->current_char;
	current_char_str[1] = '\0';
	new_buffer = ft_strjoin_free(*buffer, current_char_str);
	if (!new_buffer)
	{
		free(*buffer);
		*buffer = NULL;
		return (0);
	}
	*buffer = new_buffer;
	move_forward(lexer);
	return (1);
}
