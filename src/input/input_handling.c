/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:48:08 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/26 18:35:17 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	main_input_loop(int ac, char **av, t_minishell *shell)
{
	char		*input;

	handle_multiple_args(ac, av);
	while (1)
	{
		input = read_user_input();
		if (handle_exit_command(input, shell))
			continue ;
		if (*input)
			add_history(input);
		printf("you entered: %s\n", input);
		run_lexer(input, shell);
		shell->ast_root = parse_expression(shell, MIN_PRECEDENCE_LVL);
		read_tree(shell->ast_root, shell);
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
