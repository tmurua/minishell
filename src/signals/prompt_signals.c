/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 19:47:34 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/13 13:44:12 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	setup_sigpipe_handler(t_minishell *shell);

/* global variable to handle signals */
t_minishell	*g_shell;

/*	sets up signal handlers for the shell prompt.
	handles SIGINT (Ctrl+C) and ignores SIGQUIT (Ctrl+\). */
void	setup_prompt_signals(t_minishell *shell)
{
	setup_sigint_handler(shell);
	setup_sigquit_handler(shell);
	setup_sigpipe_handler(shell);
}

/*	setup SIGINT (Ctrl+C) signal handler for the prompt
	declare a sigaction structure to specify the action for SIGINT
	configure signal action to use handle_sigint_at_prompt
	when SIGINT received, SA_RESTART flag restarts interrupted system calls
	sigemptyset() initializes the signal mask to exclude all signals
	apply the sigaction for SIGINT, if it fails, print an error and exit */
void	setup_sigint_handler(t_minishell *shell)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint_at_prompt;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("minishell: sigaction");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
}

/*	signal handler for SIGINT (Ctrl+C) at prompt */
void	handle_sigint_at_prompt(int sig)
{
	(void)sig;
	if (g_shell && g_shell->cmd_in_execution == 0)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (g_shell && g_shell->cmd_in_execution == 1)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_redisplay();
		g_shell->cmd_in_execution = 0;
	}
}

/*	setup SIGQUIT (Ctrl+\) signal handler to ignore it at the prompt
	declare a sigaction structure to specify the action for SIGQUIT
	configure signal action to use SIG_IGN, i.e. to ignore SIGQUIT
	when SIGQUIT received, SA_RESTART flag restarts interrupted system calls
	sigemptyset() initializes the signal mask to exclude all signals
	apply the sigaction for SIGQUIT, if it fails, print an error and exit */
void	setup_sigquit_handler(t_minishell *shell)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("minishell: sigaction");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
}

void	setup_sigpipe_handler(t_minishell *shell)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN; // Ignore SIGPIPE
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGPIPE, &sa, NULL) == -1)
	{
		perror("minishell: sigaction");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
}
