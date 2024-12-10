/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:35:59 by dlemaire          #+#    #+#             */
/*   Updated: 2024/12/10 23:54:06 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	run_lexer(char *str, t_minishell *shell)
{
	t_lexer	lexer;
	t_token	*current_token;

	initialize_lexer_and_tokens(str, &lexer, shell, &current_token);
	if (process_lexer_tokens(&lexer, shell, &current_token))
		return ;
	if (handle_unclosed_quotes(&lexer, shell))
		return ;
	update_redirect_tokens(shell->tokens);
}

/* init lexer and set shell tokens and current token pointers */
void	initialize_lexer_and_tokens(char *str, t_lexer *lexer,
		t_minishell *shell, t_token **current_token)
{
	*lexer = init_lexer(str);
	shell->tokens = NULL;
	*current_token = NULL;
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

/* loop over input chars, creating tokens until EOF or error */
int	process_lexer_tokens(t_lexer *lexer, t_minishell *shell,
		t_token **current_token)
{
	t_token	*new_token;

	while (lexer->current_char != '\0')
	{
		skip_whitespace(lexer);
		new_token = get_next_token(lexer, shell);
		if (!new_token || new_token->type == TOKEN_INVALID)
			return (1);
		token_to_list(&(shell->tokens), current_token, new_token);
	}
	return (0);
}

/* check for unclosed quotes and print error if found */
int	handle_unclosed_quotes(t_lexer *lexer, t_minishell *shell)
{
	if (lexer->state == SINGLE_QUOTE_STATE
		|| lexer->state == DOUBLE_QUOTE_STATE)
	{
		ft_putstr_fd("minishell: unexpected EOF while looking for matching `",
			2);
		if (lexer->state == SINGLE_QUOTE_STATE)
			ft_putstr_fd("'", 2);
		else
			ft_putstr_fd("\"", 2);
		ft_putstr_fd("'\n", 2);
		shell->tokens = NULL;
		return (1);
	}
	return (0);
}
