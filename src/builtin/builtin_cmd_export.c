/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_export.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 08:54:22 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/20 17:39:24 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* handle the export built-in command */
int	builtin_export(char **args, t_minishell *shell)
{
	int	i;
	int	return_status;
	int	result;

	return_status = 0;
	i = 1;
	while (args[i])
	{
		result = process_export_argument(args[i], shell);
		if (result != 0)
			return_status = result;
		i++;
	}
	return (return_status);
}

/*	process a single export argument;
	split arg into name and value, validate name, set environment variable */
int	process_export_argument(const char *arg, t_minishell *shell)
{
	char	*value;

	value = ft_strchr(arg, '=');
	if (!value)
	{
		print_builtin_error("export", "invalid argument");
		return (1);
	}
	*value = '\0';
	value++;
	if (!is_valid_env_name(arg))
	{
		print_builtin_error("export", "invalid variable name");
		return (1);
	}
	return (set_env_variable(arg, value, shell));
}

/* check if string is valid environment variable name;
	i.e. name should start with alpha or '_' and contain only alnum or '_' */
int	is_valid_env_name(const char *name)
{
	if (!ft_isalpha(*name) && *name != '_')
		return (0);
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (0);
		name++;
	}
	return (1);
}

/* set an environment variable */
int	set_env_variable(const char *name, const char *value, t_minishell *shell)
{
	int		index;
	char	*new_var;
	int		len;

	index = find_env_index(name, shell);
	new_var = ft_strjoin(name, "=");
	if (!new_var)
		return (1);
	new_var = ft_strjoin_free(new_var, value);
	if (!new_var)
		return (1);
	if (index != -1)
	{
		free(shell->env[index]);
		shell->env[index] = new_var;
	}
	else
	{
		len = 0;
		while ((*shell->env)[len])
			len++;
		shell->env = realloc(*shell->env, sizeof(char *) * (len + 2));
		if (!(*shell->env))
		{
			free(new_var);
			return (1);
		}
		shell->env[len] = new_var;
		shell->env[len + 1] = NULL;
	}
	return (0);
}

/* find the index of a variable in the environment array */
int	find_env_index(const char *name, t_minishell *shell)
{
	int		i;
	char	*var_name;

	i = 0;
	while (shell->env[i])
	{
		var_name = ft_strchr(shell->env[i], '=');
		if (var_name && ft_strncmp(shell->env[i], name, var_name
				- shell->env[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}
