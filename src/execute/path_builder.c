/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 00:37:05 by dlemaire          #+#    #+#             */
/*   Updated: 2024/12/10 19:38:19 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**create_directories(t_minishell *shell);
char	*find_executable_path(char *str, char **directories,
			t_minishell *shell);

char	*build_command_path(char *str, t_minishell *shell)
{
	char	**directories;
	char	*result_path;

	if (access(str, X_OK) == 0)
		return (gc_strdup(&shell->gc_head, str));
	directories = create_directories(shell);
	if (!directories)
		return (NULL);
	result_path = find_executable_path(str, directories, shell);
	return (result_path);
}

char	**create_directories(t_minishell *shell)
{
	int		i;
	char	*path_value;
	char	**directories;

	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp("PATH=", shell->env[i], 5) == 0)
		{
			path_value = ft_strchr(shell->env[i], '/');
			if (!path_value)
				return (NULL);
			directories = gc_split(&shell->gc_head,
					shell->env[i] + 5, ':');
			if (!directories)
			{
				gc_free_all(shell->gc_head);
				exit(EXIT_FAILURE);
			}
			return (directories);
		}
		i++;
	}
	return (NULL);
}

char	*find_executable_path(char *str, char **directories, t_minishell *shell)
{
	int		i;
	char	*full_process;
	char	*full_path;

	i = 0;
	while (directories[i])
	{
		full_process = gc_strjoin(&shell->gc_head, "/", str);
		full_path = gc_strjoin(&shell->gc_head, directories[i], full_process);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
}
