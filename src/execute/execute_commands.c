/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:32:58 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/27 00:39:28 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int		count_arg_tokens(t_token *tokens);
void	add_infile_to_cmd(t_command *cmd, char *filename, t_minishell *shell);
void	add_outfile_to_cmd(t_command *cmd, char *filename, t_minishell *shell,
			int append_flag);

void	init_command(t_command *cmd, t_token *node_tokens, t_minishell *shell)
{
	int	arg_count;
	int	i;

	cmd->cmd_name = NULL;
	cmd->path = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	arg_count = count_arg_tokens(node_tokens);
	i = 0;
	cmd->args = gc_calloc(&shell->gc_head, arg_count + 2, sizeof(char *));
	if (!cmd->args)
	{
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	while (node_tokens)
	{
		if (node_tokens->type == TOKEN_BUILTIN_CMD)
		{
			cmd->cmd_name = gc_strdup(&shell->gc_head, node_tokens->value);
			if (!cmd->cmd_name)
			{
				gc_free_all(shell->gc_head);
				exit(EXIT_FAILURE);
			}
			cmd->args[i] = gc_strdup(&shell->gc_head, node_tokens->value);
			if (!cmd->args[i++])
			{
				gc_free_all(shell->gc_head);
				exit(EXIT_FAILURE);
			}
		}
		if (node_tokens->type == TOKEN_EXTERN_CMD)
		{
			cmd->cmd_name = gc_strdup(&shell->gc_head, node_tokens->value);
			if (!cmd->cmd_name)
			{
				gc_free_all(shell->gc_head);
				exit(EXIT_FAILURE);
			}
			cmd->args[i] = gc_strdup(&shell->gc_head, node_tokens->value);
			if (!cmd->args[i++])
			{
				gc_free_all(shell->gc_head);
				exit(EXIT_FAILURE);
			}
			cmd->path = build_command_path(cmd->cmd_name, shell);
		}
		if (node_tokens->type == TOKEN_ARGUMENT)
		{
			cmd->args[i] = gc_strdup(&shell->gc_head, node_tokens->value);
			if (!cmd->args[i++])
			{
				gc_free_all(shell->gc_head);
				exit(EXIT_FAILURE);
			}
		}
		if (node_tokens->type == TOKEN_REDIRECT_IN)
		{
			if (node_tokens->next->type == TOKEN_FILENAME)
			{
				add_infile_to_cmd(cmd, node_tokens->next->value, shell);
				node_tokens = node_tokens->next;
			}
		}
		if (node_tokens->type == TOKEN_REDIRECT_OUT)
		{
			if (node_tokens->next->type == TOKEN_FILENAME)
			{
				add_outfile_to_cmd(cmd, node_tokens->next->value, shell, 0);
				node_tokens = node_tokens->next;
			}
		}
		if (node_tokens->type == TOKEN_REDIRECT_APPEND)
		{
			if (node_tokens->next->type == TOKEN_FILENAME)
			{
				add_outfile_to_cmd(cmd, node_tokens->next->value, shell, 1);
				node_tokens = node_tokens->next;
			}
		}
		node_tokens = node_tokens->next;
	}
	cmd->args[i] = NULL;
	shell->cmd = cmd;
}

int	count_arg_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_ARGUMENT)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

void	add_infile_to_cmd(t_command *cmd, char *filename, t_minishell *shell)
{
	t_files	*new_infile;
	t_files	*current;

	new_infile = gc_calloc(&shell->gc_head, 1, sizeof(t_files));
	if (!new_infile)
	{
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	new_infile->delim = NULL;
	new_infile->next = NULL;
	new_infile->fd = open(filename, O_RDONLY, 0644);
	if (new_infile->fd < 0)
	{
		perror(filename);
		new_infile->fd = open("/dev/null", O_RDONLY);
	}
	if (!cmd->infile)
		cmd->infile = new_infile;
	else
	{
		current = cmd->infile;
		while (current->next)
			current = current->next;
		current->next = new_infile;
	}
}

void	add_outfile_to_cmd(t_command *cmd, char *filename, t_minishell *shell,
			int append_flag)
{
	t_files	*new_outfile;
	t_files	*current;

	new_outfile = gc_calloc(&shell->gc_head, 1, sizeof(t_files));
	if (!new_outfile)
	{
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	new_outfile->delim = NULL;
	new_outfile->next = NULL;
	if (append_flag)
		new_outfile->fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		new_outfile->fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (new_outfile->fd < 0)
	{
		perror(filename);
		new_outfile->fd = open("/dev/null", O_WRONLY);
	}
	if (!cmd->outfile)
		cmd->outfile = new_outfile;
	else
	{
		current = cmd->outfile;
		while (current->next)
			current = current->next;
		current->next = new_outfile;
	}
}

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
		ignore_signal_handlers(shell);
		execute_external_cmd(args, env);
		setup_prompt_signals(shell);
	}
}*/

/* count_tokens, allocate_args_array, copy_token_values */
// char	**tokens_to_args(t_token *tokens, t_minishell *shell)
// {
// 	char	**args;
// 	int		token_count;

// 	token_count = count_tokens(tokens);
// 	args = allocate_args_array(token_count, shell);
// 	if (!args)
// 		return (NULL);
// 	if (copy_token_values(tokens, args, shell) == -1)
// 		return (NULL);
// 	return (args);
// }

/* malloc array with count +1 elements & init last to NULL, to mark end of it */
// char	**allocate_args_array(int count, t_minishell *shell)
// {
// 	char	**args;

// 	args = gc_calloc(&shell->gc_head, count + 1, sizeof(char *));
// 	if (!args)
// 	{
// 		perror("minishell: allocate_args_array");
// 		return (NULL);
// 	}
// 	args[count] = NULL;
// 	return (args);
// }

/* duplicates each token's value using 'ft_strdup' and stores it in the array */
// int	copy_token_values(t_token *tokens, char **args, t_minishell *shell)
// {
// 	int		i;
// 	t_token	*current;

// 	i = 0;
// 	current = tokens;
// 	while (current)
// 	{
// 		args[i] = gc_strdup(&shell->gc_head, current->value);
// 		if (!args[i])
// 		{
// 			perror("minishell: copy_token_values");
// 			return (-1);
// 		}
// 		current = current->next;
// 		i++;
// 	}
// 	return (0);
// }
