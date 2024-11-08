/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_state_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 16:42:16 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/08 14:25:56 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	collects the next token from the lexer based on its current state */
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

/*	if whitespace found, end current token; if ' or " found, change STATE;
	if $ found, handle var expansion; otherwise advance_and_append each char */
int	handle_default_state(t_lexer *lexer, char **buffer)
{
	if (ft_iswhitespace(lexer->current_char))
		return (TOKEN_COMPLETE);
	else if (lexer->current_char == '\'')
	{
		lexer->state = SINGLE_QUOTE_STATE;
		advance_lexer_char(lexer);
	}
	else if (lexer->current_char == '"')
	{
		lexer->state = DOUBLE_QUOTE_STATE;
		advance_lexer_char(lexer);
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

/* if ' found, change to DEFAULT_STATE; advance_and_append all chars */
int	handle_single_quote_state(t_lexer *lexer, char **buffer)
{
	if (lexer->current_char == '\'')
	{
		lexer->state = DEFAULT_STATE;
		advance_lexer_char(lexer);
	}
	else
	{
		if (!advance_and_append(lexer, buffer))
			return (TOKEN_ERROR);
	}
	return (TOKEN_CONTINUE);
}

/* if " found, change to DEFAULT_STATE; handle $; or advance_and_append*/
int	handle_double_quote_state(t_lexer *lexer, char **buffer)
{
	if (lexer->current_char == '"')
	{
		lexer->state = DEFAULT_STATE;
		advance_lexer_char(lexer);
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
	advance_lexer_char(lexer);
	return (1);
}
