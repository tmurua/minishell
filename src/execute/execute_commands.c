/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:32:58 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/04 02:14:32 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*	initialize command based on tokens */
void	init_command(t_command *cmd, t_token *node_tokens, t_minishell *shell)
{
	int	arg_count;
	int	i;

	initialize_command_struct(cmd);
	cmd->has_heredoc = 0;
	arg_count = count_arg_tokens(node_tokens);
	i = 0;
	cmd->args = gc_calloc(&shell->gc_head, arg_count + 2, sizeof(char *));
	if (!cmd->args)
	{
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	while (node_tokens)
		node_tokens = process_token(cmd, node_tokens, shell, &i);
	cmd->args[i] = NULL;
	shell->cmd = cmd;
}

void	initialize_command_struct(t_command *cmd)
{
	cmd->cmd_name = NULL;
	cmd->path = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
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

void	add_heredoc_to_cmd(t_command *cmd, char *delimiter, t_minishell *shell)
{
	t_files	*new_infile;
	t_files	*current;

	cmd->has_heredoc = 1;
	new_infile = gc_calloc(&shell->gc_head, 1, sizeof(t_files));
	if (!new_infile)
	{
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	new_infile->delim = gc_strdup(&shell->gc_head, delimiter);
	new_infile->next = NULL;
	new_infile->fd = get_last_heredoc_fd((t_files*)(shell->heredocs->content));
	if (new_infile->fd < 0)
	{
		perror("heredoc");
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
	// clear heredocs list?? with ft_lstclear()??
}


int	process_argument(t_command *cmd, t_token *token, t_minishell *shell, int i)
{
	cmd->args[i] = gc_strdup(&shell->gc_head, token->value);
	if (!cmd->args[i])
	{
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	return (i + 1);
}

/*	process a single token and update cmd struct
	i: pointer to current index in cmd->args.
	return the next token to process */
t_token	*process_token(t_command *cmd, t_token *token, t_minishell *shell,
		int *i)
{
	if (token->type == TOKEN_BUILTIN_CMD)
	{
		*i = process_builtin_cmd(cmd, token, shell, *i);
		token = token->next;
	}
	else if (token->type == TOKEN_EXTERN_CMD)
	{
		*i = process_extern_cmd(cmd, token, shell, *i);
		token = token->next;
	}
	else if (token->type == TOKEN_ARGUMENT)
	{
		*i = process_argument(cmd, token, shell, *i);
		token = token->next;
	}
	else if (token->type == TOKEN_REDIRECT_IN)
		token = process_redirect_in(cmd, token, shell);
	else if (token->type == TOKEN_HEREDOC)
	{
		if (token->next->type == TOKEN_HEREDOC_DELIMITER)
		{
			add_heredoc_to_cmd(cmd, token->next->value, shell);
			token = token->next;
		}
	}
	else if (token->type == TOKEN_REDIRECT_OUT)
		token = process_redirect_out(cmd, token, shell);
	else if (token->type == TOKEN_REDIRECT_APPEND)
		token = process_redirect_append(cmd, token, shell);
	else
		token = token->next;
	return (token);
}

int	get_last_heredoc_fd(t_files *heredocs)
{
	t_files	*current;

	if (!heredocs)
		return (-1);
	current = heredocs;
	while (current->next)
		current = current->next;
	return (current->fd);
}
