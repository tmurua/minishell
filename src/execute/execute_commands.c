/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:32:58 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/20 19:04:45 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* token_to_args: convert linked list of tokens into array of args */
/*void	execute_command(t_token *tokens, t_minishell *shell)
{
	char	**args;

	args = tokens_to_args(tokens, shell);
	if (!args)
		return ;
	if (is_builtin_command(args[0]))
		execute_builtin(args, env);
	else
	{
		ignore_signal_handlers();
		execute_external_cmd(args, env, tokens);
		setup_prompt_signals();
	}
	free_arguments(args);
}*/

/* count_tokens, allocate_args_array, copy_token_values */
char	**tokens_to_args(t_token *tokens, t_minishell *shell)
{
	char	**args;
	int		token_count;

	token_count = count_tokens(tokens);
	args = allocate_args_array(token_count, shell);
	if (!args)
		return (NULL);
	if (copy_token_values(tokens, args, shell) == -1)
	{
		free_arguments(args);
		return (NULL);
	}
	return (args);
}

/* malloc array with count +1 elements & init last to NULL, to mark end of it */
char	**allocate_args_array(int count, t_minishell *shell)
{
	char	**args;

	args = gc_calloc(&shell->gc_head, count + 1, sizeof(char *));
	if (!args)
	{
		perror("minishell: allocate_args_array");
		return (NULL);
	}
	args[count] = NULL;
	return (args);
}

/* duplicates each token's value using 'ft_strdup' and stores it in the array */
int	copy_token_values(t_token *tokens, char **args, t_minishell *shell)
{
	int		i;
	t_token	*current;

	i = 0;
	current = tokens;
	while (current)
	{
		args[i] = gc_strdup(&shell->gc_head, current->value);
		if (!args[i])
		{
			perror("minishell: copy_token_values");
			return (-1);
		}
		current = current->next;
		i++;
	}
	return (0);
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

