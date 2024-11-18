/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_unset.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:37:10 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/18 18:39:05 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* handle unset built-in command */
int	builtin_unset(char **args, char ***env)
{
	int	i;
	int	return_status;
	int	result;

	return_status = 0;
	i = 1;
	while (args[i])
	{
		result = process_unset_argument(args[i], env);
		if (result != 0)
			return_status = result;
		i++;
	}
	return (return_status);
}

/* process a single unset argument */
int	process_unset_argument(const char *arg, char ***env)
{
	if (!is_valid_env_name(arg))
	{
		print_builtin_error("unset", "invalid variable name");
		return (1);
	}
	return (unset_env_variable(arg, env));
	return (0);
}

// Function to remove an environment variable
int	unset_env_variable(const char *name, char ***env)
{
	int	index;
	int	i;

	index = find_env_index(name, *env);
	if (index == -1)
		return (0); // Variable not found, no error
	free((*env)[index]);
	i = index;
	while ((*env)[i + 1])
	{
		(*env)[i] = (*env)[i + 1];
		i++;
	}
	(*env)[i] = NULL;
	(*env) = realloc(*env, sizeof(char *) * (i + 1));
	if (!(*env))
		return (1);
	return (0);
}
