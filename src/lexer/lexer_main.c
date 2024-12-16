/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:35:59 by dlemaire          #+#    #+#             */
/*   Updated: 2024/12/17 00:26:39 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	run_lexer(char *str, t_minishell *shell)
{
	t_lexer	lexer;

	initialize_lexer_and_tokens(str, &lexer, shell);
	if (process_lexer_tokens(&lexer, shell))
		return ;
	if (handle_unclosed_quotes(&lexer, shell))
		return ;
	//update_redirect_tokens(shell->tokens, shell);
}

/* init lexer and set shell tokens and current token pointers */
void	initialize_lexer_and_tokens(char *str, t_lexer *lexer,
		t_minishell *shell)
{
	*lexer = init_lexer(str);
	shell->tokens = NULL;
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
int	process_lexer_tokens(t_lexer *lexer, t_minishell *shell)
{
	t_token	*new_token;
	t_token	*cl_parenthesis_token;
	size_t	len;
	int		cl_parenthesis_counter;

	while (lexer->current_char != '\0')
	{
		skip_whitespace(lexer);
		new_token = get_next_token(lexer, shell);
		if (!new_token || new_token->type == TOKEN_INVALID)
			return (1);
		cl_parenthesis_counter = 0;
		while (1)
		{
			len = ft_strlen(new_token->value);
			if (len > 0 && new_token->value[len - 1] == ')')
			{
				cl_parenthesis_counter++;
				new_token->value[len - 1] = '\0';
			}
			else
			{
				if (*new_token->value != '\0')
					token_to_list(&(shell->tokens), new_token);
				while (cl_parenthesis_counter)
				{
					cl_parenthesis_token = create_token(TOKEN_CL_PARENTHESIS, ")", shell);
					token_to_list(&(shell->tokens), cl_parenthesis_token);
					cl_parenthesis_counter--;
				}
				break ;
			}
		}
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
		shell->last_exit_status = 2;
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
