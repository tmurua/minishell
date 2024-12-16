/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:19:43 by dlemaire          #+#    #+#             */
/*   Updated: 2024/12/16 18:59:06 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int					g_received_signal;

int	main(int ac, char *av[], char *envp[])
{
	t_minishell	shell;

	shell.gc_head = NULL;
	shell.heredocs = NULL;
	shell.env = duplicate_env(envp, &shell);
	shell.last_exit_status = 0;
	g_received_signal = 0;
	if (!shell.env)
	{
		perror("minishell: duplicate_env");
		return (1);
	}
	setup_prompt_signals(&shell);
	main_input_loop(ac, av, &shell);
	gc_free_all(shell.gc_head);
	return (0);
}
