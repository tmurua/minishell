/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:48:08 by tmurua            #+#    #+#             */
/*   Updated: 2024/10/31 12:25:49 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	main_input_loop(int ac, char **av, char **envp)
{
	char	*input;
	char	**args;
	t_token	*tokens;
	int		i;

	handle_multiple_args(ac, av);
	while (1)
	{
		input = read_user_input();
		if (input == NULL || !ft_strncmp(input, "exit", 5))
		{
			free(input);
			rl_clear_history();
			exit (0);
		}
		if (*input)
			add_history(input);
		printf("you entered: %s\n", input);
		tokens = run_lexer(input);
		//parse_tokens(tokens);
		args = ft_split(input, ' ');
		if (execute_builtin(args, envp) == -1)
		{
			// execute_external_command(args);
		}
		i = 0;
		while (args[i])
		{
			free(args[i]);
			i++;
		}
		free(args);
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
