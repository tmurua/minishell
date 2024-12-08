/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:48:08 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/08 22:44:26 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	main_input_loop(int ac, char **av, t_minishell *shell)
{
	char		*input;

	handle_multiple_args(ac, av, shell);
	while (1)
	{
		input = read_user_input();
		if (input == NULL)
		{
			printf("exit\n");
			rl_clear_history();
			gc_free_all(shell->gc_head);
			exit(shell->last_exit_status);
		}
		if (handle_exit_command(input, shell))
			continue ;
		if (*input)
			add_history(input);
		heredoc_scan(input, shell);
		run_lexer(input, shell);
		shell->ast_root = parse_expression(shell, MIN_PRECEDENCE_LVL);
		read_tree(shell->ast_root, shell);
	}
}

void	handle_multiple_args(int ac, char **av, t_minishell *shell)
{
	if (ac != 1 || av[1])
	{
		printf("minishell does not accept arguments\n");
		gc_free_all(shell->gc_head);
		exit(0);
	}
}

char	*read_user_input(void)
{
	char	*input;

	input = readline("minishell$> ");
	return (input);
}
