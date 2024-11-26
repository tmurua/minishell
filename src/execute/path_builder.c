/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 00:37:05 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/27 00:37:58 by dlemaire         ###   ########.fr       */
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
	{
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	result_path = find_executable_path(str, directories, shell);
	return (result_path);
}

char	**create_directories(t_minishell *shell)
{
	int		i;
	char	**directories;

	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp("PATH=", shell->env[i], 5) == 0)
		{
			directories = ft_split(ft_strchr(shell->env[i], '/'), ':');
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