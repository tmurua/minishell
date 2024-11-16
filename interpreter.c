/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 16:26:15 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/16 00:07:13 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// traverses the AST starting from root and following the hierarchy to execute
// the commands in the correct order

#include "minishell.h"

/*
void	read_tree(t_ast_node *root)
{
	char	**args;

	if (root->type == NODE_PIPE)
	{
		// add pipex
		printf("TODO: execute pipe logic from pipex\n");
	}
	else if (root->type == NODE_COMMAND)
	{
		if (root->tokens->type == TOKEN_BUILTIN_CMD)
			printf("implement echo\n");
		else
			printf("execute executable\n");
	}
}*/
/* main function to read and execute the parsed AST */
void	read_tree(t_ast_node *root, char **envp)
{
	if (root == NULL)
		return ;
	evaluate_and_execute(root, envp);
}

/*	helper function to evaluate and execute the AST; handle the execution of
	different types of AST nodes (commands, Pipes, logical operators) */
int	evaluate_and_execute(t_ast_node *node, char **envp)
{
	int			left_result;
	t_command	cmd;

	if (node->type == NODE_COMMAND)
	{
		//execute_command_node(node->tokens, envp);
		init_command(&cmd, node->tokens, envp);
		if (node->tokens->type == TOKEN_EXTERN_CMD)
			run_program(&cmd);
		else if (node->tokens->type == TOKEN_BUILTIN_CMD)
			execute_builtin(cmd.args, envp);
		return (0);
	}
	else if (node->type == NODE_PIPE)
	{
		//evaluate_and_execute(node->left, envp);
		//evaluate_and_execute(node->right, envp);
		init_pipe(node, envp);
		return (0);
	}
	else if (node->type == NODE_AND || node->type == NODE_OR)
	{
		left_result = evaluate_and_execute(node->left, envp);
		if ((node->type == NODE_AND && left_result == 0)
			|| (node->type == NODE_OR && left_result != 0))
			evaluate_and_execute(node->right, envp);
		return (left_result);
	}
	return (-1);
}

/*	helper function to execute a command node;convert tokens to
	args and execute either builtin cmd or external command */
void	execute_command_node(t_token *tokens, char **envp)
{
	char	**args;

	args = tokens_to_args(tokens);
	if (args == NULL)
		return ;
	if (tokens->type == TOKEN_BUILTIN_CMD)
		execute_builtin(args, envp);
	else
		execute_external_cmd(args, envp, tokens);
	free_arguments(args);
}
