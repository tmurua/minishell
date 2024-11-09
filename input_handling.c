/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:48:08 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/09 20:12:23 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	main_input_loop(int ac, char **av, char **envp)
{
	char	*input;
	t_token	*tokens;

	handle_multiple_args(ac, av);
	while (1)
	{
		input = read_user_input();
		if (handle_exit_command(input))
			continue ;
		if (*input)
			add_history(input);
		printf("you entered: %s\n", input);
		tokens = run_lexer(input);
		parse_expression(&tokens, 0); // create macro (no magic number)
		execute_command(tokens, envp);
		cleanup_input(tokens, input);
	}
}

void	handle_multiple_args(int ac, char **av)
{
	if (ac != 1 || av[1])
	{
		printf("minishell does not accept arguments\n");
		exit(0);
	}
}

char	*read_user_input(void)
{
	char	*input;

	input = readline("minishell$> ");
	return (input);
}

void	cleanup_input(t_token *tokens, char *input)
{
	if (tokens)
		free_tokens(tokens);
	if (input)
		free(input);
}
