/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:15:52 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/29 17:16:24 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*	resets signal handlers to their default behavior.
	used in child processes before executing external commands */
void	reset_signal_handlers(t_minishell *shell)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
	{
		perror("minishell: signal");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
	{
		perror("minishell: signal");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
}

/*	ignores SIGINT and SIGQUIT in parent process during command execution,
	preventing shell from being interrupted by these signals */
void	ignore_signal_handlers(t_minishell *shell)
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
	{
		perror("minishell: signal");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		perror("minishell: signal");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
}
