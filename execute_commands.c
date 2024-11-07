/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:32:58 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/07 10:34:54 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* needs to be changed to use tokens instead of args */
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
