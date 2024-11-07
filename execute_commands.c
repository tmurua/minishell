/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:32:58 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/07 16:48:50 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_token *tokens, char **envp)
{
	char	**args;

	args = tokens_to_args(tokens);
	if (!args)
		return ;
	if (is_builtin_command(args[0]))
		execute_builtin(args, envp);
	else
	{
		ignore_signal_handlers();
		execute_external_cmd(args, envp, tokens);
		setup_prompt_signals();
	}
	free_arguments(args);
}

char	**tokens_to_args(t_token *tokens)
{
	t_token	*current;
	char	**args;
	int		count;
	int		i;

	count = 0;
	current = tokens;
	while (current)
	{
		count++;
		current = current->next;
	}
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	current = tokens;
	i = 0;
	while (current)
	{
		args[i] = ft_strdup(current->value);
		if (!args[i])
		{
			while (i > 0)
				free(args[--i]);
			free(args);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	args[i] = NULL;
	return (args);
}

void	free_arguments(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

