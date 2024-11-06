/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:48:08 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/06 11:59:10 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	main_input_loop(int ac, char **av, char **envp)
{
	char	*input;
	char	**args;
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
		// parse_tokens(tokens);
		args = ft_split(input, ' ');
		execute_command(args, envp);
		cleanup_input(args, tokens, input);
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
	if (is_builtin_command(args[0]))
		execute_builtin(args, envp);
	else
	{
		ignore_signal_handlers();
		execute_external_cmd(args, envp);
		setup_prompt_signals();
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
