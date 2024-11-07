/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 16:42:16 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/07 10:23:25 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/*	create & return buffer str to collect chars from lexer input to form a token
	if current_char is ' or " collect_quoted_token(), else append char 1 by 1 */
char	*collect_token(t_lexer *lexer)
{
	char	*buffer;

	buffer = ft_strdup("");
	if (!buffer)
		return (NULL);
	while (lexer->current_char != '\0' && !ft_iswhitespace(lexer->current_char))
	{
		if (lexer->current_char == '\'' || lexer->current_char == '"')
			collect_quoted_token(lexer, &buffer);
		else
			advance_and_append(lexer, &buffer);
		if (!buffer)
			return (NULL);
	}
	return (buffer);
}

/*	if current_char was ' or " = quote_char, move lexer forward 1 char, append
	next chars normally until another ' or "" is found to move forward & end */
void	collect_quoted_token(t_lexer *lexer, char **buffer)
{
	char	quote_char;

	quote_char = lexer->current_char;
	move_forward(lexer);
	while (lexer->current_char != '\0' && lexer->current_char != quote_char)
		advance_and_append(lexer, buffer);
	if (lexer->current_char == quote_char)
		move_forward(lexer);
}

/*	current_char_str: temp str to hold current_char as a null-terminated str
	join current buffer with current_char_str at the end, move lexer forward */
void	advance_and_append(t_lexer *lexer, char **buffer)
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
		return ;
	}
	*buffer = new_buffer;
	move_forward(lexer);
}
