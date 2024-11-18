/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 19:47:34 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/18 18:39:13 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*	sets up signal handlers for the shell prompt.
	handles SIGINT (Ctrl+C) and ignores SIGQUIT (Ctrl+\). */
void	setup_prompt_signals(void)
{
	setup_sigint_handler();
	setup_sigquit_handler();
}

/*	setup SIGINT (Ctrl+C) signal handler for the prompt
	declare a sigaction structure to specify the action for SIGINT
	configure signal action to use handle_sigint_at_prompt
	when SIGINT received, SA_RESTART flag restarts interrupted system calls
	sigemptyset() initializes the signal mask to exclude all signals
	apply the sigaction for SIGINT, if it fails, print an error and exit */
void	setup_sigint_handler(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint_at_prompt;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("minishell: sigaction");
		exit(EXIT_FAILURE);
	}
}

/*	signal handler for SIGINT (Ctrl+C) at prompt
	writes \n, clear current line, goes to next line, and redisplays prompt */
void	handle_sigint_at_prompt(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/*	setup SIGQUIT (Ctrl+\) signal handler to ignore it at the prompt
	declare a sigaction structure to specify the action for SIGQUIT
	configure signal action to use SIG_IGN, i.e. to ignore SIGQUIT
	when SIGQUIT received, SA_RESTART flag restarts interrupted system calls
	sigemptyset() initializes the signal mask to exclude all signals
	apply the sigaction for SIGQUIT, if it fails, print an error and exit */
void	setup_sigquit_handler(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("minishell: sigaction");
		exit(EXIT_FAILURE);
	}
}

/*	resets signal handlers to their default behavior.
	used in child processes before executing external commands */
void	reset_signal_handlers(void)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
	{
		perror("minishell: signal");
		exit(EXIT_FAILURE);
	}
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
	{
		perror("minishell: signal");
		exit(EXIT_FAILURE);
	}
}

/*	ignores SIGINT and SIGQUIT in parent process during command execution,
	preventing shell from being interrupted by these signals */
void	ignore_signal_handlers(void)
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
	{
		perror("minishell: signal");
		exit(EXIT_FAILURE);
	}
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		perror("minishell: signal");
		exit(EXIT_FAILURE);
	}
}
