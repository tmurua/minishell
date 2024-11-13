/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_export.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 08:54:22 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/13 14:36:04 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* handle the export built-in command */
int	builtin_export(char **args)
{
	int	i;
	int	return_status;
	int	result;

	return_status = 0;
	i = 1;
	while (args[i])
	{
		result = process_export_argument(args[i]);
		if (result != 0)
		{
			return_status = result;
		}
		i++;
	}
	return (return_status);
}

/*	process a single export argument;
	split arg into name and value, validate name, set environment variable */
int	process_export_argument(const char *arg)
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
	return (set_env_variable(arg, value));
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
int	set_env_variable(const char *name, const char *value)
{
	if (setenv(name, value, 1) != 0)
	{
		perror("minishell: setenv");
		return (1);
	}
	return (0);
}

/* handle unset built-in command */
int	builtin_unset(char **args)
{
	int	i;
	int	return_status;
	int	result;

	return_status = 0;
	i = 1;
	while (args[i])
	{
		result = process_unset_argument(args[i]);
		if (result != 0)
			return_status = result;
		i++;
	}
	return (return_status);
}

/* process a single unset argument */
int	process_unset_argument(const char *arg)
{
	if (!is_valid_env_name(arg))
	{
		print_builtin_error("unset", "invalid variable name");
		return (1);
	}
	if (unsetenv(arg) != 0)
	{
		perror("minishell: unsetenv");
		return (1);
	}
	return (0);
}
