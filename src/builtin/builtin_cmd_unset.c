/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_unset.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:37:10 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/20 17:43:21 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* handle unset built-in command */
int	builtin_unset(char **args, t_minishell *shell)
{
	int	i;
	int	return_status;
	int	result;

	return_status = 0;
	i = 1;
	while (args[i])
	{
		result = process_unset_argument(args[i], shell);
		if (result != 0)
			return_status = result;
		i++;
	}
	return (return_status);
}

/* process a single unset argument */
int	process_unset_argument(const char *arg, t_minishell *shell)
{
	if (!is_valid_env_name(arg))
	{
		print_builtin_error("unset", "invalid variable name");
		return (1);
	}
	return (unset_env_variable(arg, shell));
	return (0);
}

// Function to remove an environment variable
int	unset_env_variable(const char *name, t_minishell *shell)
{
	int	index;
	int	i;

	index = find_env_index(name, shell);
	if (index == -1)
		return (0); // Variable not found, no error
	free(shell->env[index]);
	i = index;
	while (shell->env[i + 1])
	{
		shell->env[i] = shell->env[i + 1];
		i++;
	}
	shell->env[i] = NULL;
	shell->env = realloc(*shell->env, sizeof(char *) * (i + 1));
	if (!(*shell->env))
		return (1);
	return (0);
}
