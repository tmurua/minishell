/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 17:02:08 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/13 01:56:41 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_tokens(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/*	adds a new token to the end of the tokens linked list */
void	token_to_list(t_token **tokens, t_token **current, t_token *new)
{
	if (!*tokens)
		*tokens = new;
	else
		(*current)->next = new;
	*current = new;
}

/* mark next token as filename if the current token is redirect type */
void	update_redirect_tokens(t_token *tokens, t_minishell *shell)
{
	while (tokens)
	{
		if ((tokens->type == TOKEN_REDIRECT_IN
				|| tokens->type == TOKEN_REDIRECT_OUT
				|| tokens->type == TOKEN_REDIRECT_APPEND)
			&& tokens->next)
			tokens->next->type = TOKEN_FILENAME;
		if (tokens->type == TOKEN_HEREDOC)
		{
			if (tokens->next)
				tokens->next->type = TOKEN_HEREDOC_DELIMITER;
			else
			{
				ft_putstr_fd("minishell: syntax error near unexpected token\n",
					2);
				shell->last_exit_status = 2;
			}
		}
		tokens = tokens->next;
	}
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
