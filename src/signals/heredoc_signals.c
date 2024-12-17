/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 04:24:57 by dlemaire          #+#    #+#             */
/*   Updated: 2024/12/17 06:45:16 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	setup_heredoc_signals(t_minishell *shell)
{
	setup_sigint_handler_at_heredoc(shell);
	setup_sigquit_handler(shell);
	setup_sigpipe_handler(shell);
}

void	setup_sigint_handler_at_heredoc(t_minishell *shell)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint_at_heredoc;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("minishell: sigaction");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
}

void	handle_sigint_at_heredoc(int sig)
{
	rl_redisplay();
	g_received_signal = sig;
}
