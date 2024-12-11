/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 08:54:22 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/11 19:24:09 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* handle the export built-in command */
int	builtin_export(char **args, t_minishell *shell)
{
	int	i;
	int	return_status;
	int	result;

	if (!args[1])
		return (print_export_without_args(shell));
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

/* set an environment variable */
int	set_env_variable(const char *name, const char *value, t_minishell *shell)
{
	int		index;
	char	*new_var;

	index = find_env_index(name, shell);
	new_var = create_env_string(name, value, shell);
	if (!new_var)
		return (1);
	if (index != -1)
		shell->env[index] = new_var;
	else
	{
		if (add_env_variable(new_var, shell) != 0)
			return (1);
	}
	return (0);
}

/* create environment string in the format NAME=VALUE */
char	*create_env_string(const char *name, const char *value,
		t_minishell *shell)
{
	char	*env_string;

	env_string = gc_strjoin(&shell->gc_head, name, "=");
	if (!env_string)
		return (NULL);
	env_string = gc_strjoin(&shell->gc_head, env_string, value);
	if (!env_string)
		return (NULL);
	return (env_string);
}

int	add_env_variable(char *new_var, t_minishell *shell)
{
	int		len;
	int		i;
	char	**new_env;

	len = 0;
	while (shell->env[len])
		len++;
	new_env = gc_calloc(&shell->gc_head, len + 2, sizeof(char *));
	if (!new_env)
		return (1);
	i = 0;
	while (i < len)
	{
		new_env[i] = shell->env[i];
		i++;
	}
	new_env[len] = new_var;
	new_env[len + 1] = NULL;
	shell->env = new_env;
	return (0);
}
