/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:35:59 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/29 17:21:23 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	run_lexer(char *str, t_minishell *shell)
{
	t_lexer	lexer;
	t_token	*current_token;
	t_token	*new_token;

	lexer = init_lexer(str);
	shell->tokens = NULL;
	current_token = NULL;
	while (lexer.current_char != '\0')
	{
		skip_whitespace(&lexer);
		new_token = get_next_token(&lexer, shell);
		if (!new_token || new_token->type == TOKEN_INVALID)
			return ;
		token_to_list(&(shell->tokens), &current_token, new_token);
	}
	update_redirect_tokens(shell->tokens);
}

/*	init lexer with input str, set pos, state, and establish cmd is expected */
t_lexer	init_lexer(const char *arg)
{
	t_lexer	lexer;

	lexer.str = arg;
	lexer.pos = 0;
	lexer.current_char = arg[0];
	lexer.state = DEFAULT_STATE;
	lexer.command_expected = 1;
	return (lexer);
}

/*	skip all leading whitespace characters in the input */
void	skip_whitespace(t_lexer *lexer)
{
	while (lexer->current_char != '\0' && ft_iswhitespace(lexer->current_char))
		advance_lexer_char(lexer);
}

/* moves lexer forward by one char, update current_char based on new position */
void	advance_lexer_char(t_lexer *lexer)
{
	lexer->pos++;
	if (lexer->pos < ft_strlen(lexer->str))
		lexer->current_char = lexer->str[lexer->pos];
	else
		lexer->current_char = '\0';
}

/*	after skipping any leading whitespace, collect the next token, determine
	its type (TOKEN_BUILTIN_CMD,TOKEN_PIPE, etc) and return * to new token */
t_token	*get_next_token(t_lexer *lexer, t_minishell *shell)
{
	char			*value;
	t_token_type	type;
	t_token			*new_token;

	skip_whitespace(lexer);
	if (lexer->current_char == '\0')
		return (NULL);
	if (is_special_character(lexer))
		type = handle_special_char_token(lexer, &value, shell);
	else
		type = handle_regular_token(lexer, &value, shell);
	if (type == TOKEN_INVALID)
		return (NULL);
	new_token = create_token(type, value, shell);
	return (new_token);
}
