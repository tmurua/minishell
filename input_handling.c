/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:48:08 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/03 20:56:42 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	main_input_loop(int ac, char **av, char **envp)
{
	char	*input;
	char	**args;
	t_token	*tokens;
	(void)ac;
	(void)av;
	// handle_multiple_args(ac, av);
	while (1)
	{
		input = read_user_input();
		if (handle_exit_command(input))
			continue ;
		if (*input)
			add_history(input);
		printf("you entered: %s\n", input);
		tokens = run_lexer(input);
		//parse_tokens(tokens);
		free_tokens(tokens);
		free(input);
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

void	execute_command(char **args, char **envp)
{
	if (execute_builtin(args, envp) == -1)
	{
		// execute_external_command(args);
	}
}

void	cleanup_input(char **args, t_token *tokens, char *input)
{
	int	i;

	if (args)
	{
		i = 0;
		while (args[i])
		{
			free(args[i]);
			i++;
		}
		free(args);
	}
	free_tokens(tokens);
	free(input);
}
