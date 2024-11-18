/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_export.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 08:54:22 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/18 18:39:03 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* handle the export built-in command */
int	builtin_export(char **args, char ***env)
{
	int	i;
	int	return_status;
	int	result;

	return_status = 0;
	i = 1;
	while (args[i])
	{
		result = process_export_argument(args[i], env);
		if (result != 0)
			return_status = result;
		i++;
	}
	return (return_status);
}

/*	process a single export argument;
	split arg into name and value, validate name, set environment variable */
int	process_export_argument(const char *arg, char ***env)
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
	return (set_env_variable(arg, value, env));
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
int	set_env_variable(const char *name, const char *value, char ***env)
{
	int		index;
	char	*new_var;
	int		len;

	index = find_env_index(name, *env);
	new_var = ft_strjoin(name, "=");
	if (!new_var)
		return (1);
	new_var = ft_strjoin_free(new_var, value);
	if (!new_var)
		return (1);
	if (index != -1)
	{
		free((*env)[index]);
		(*env)[index] = new_var;
	}
	else
	{
		len = 0;
		while ((*env)[len])
			len++;
		(*env) = realloc(*env, sizeof(char *) * (len + 2));
		if (!(*env))
		{
			free(new_var);
			return (1);
		}
		(*env)[len] = new_var;
		(*env)[len + 1] = NULL;
	}
	return (0);
}

/* find the index of a variable in the environment array */
int	find_env_index(const char *name, char **env)
{
	int		i;
	char	*var_name;

	i = 0;
	while (env[i])
	{
		var_name = ft_strchr(env[i], '=');
		if (var_name && ft_strncmp(env[i], name, var_name - env[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}
