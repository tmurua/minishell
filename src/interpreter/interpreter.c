/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 16:26:15 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/29 18:12:38 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* main function to read and execute the parsed AST */
void	read_tree(t_ast_node *root, t_minishell *shell)
{
	if (root == NULL)
		return ;
	evaluate_and_execute(root, shell);
}

/*	handle execution of diffrent types of AST nodes
	i.e. commands, pipes, or logical operators */
int	evaluate_and_execute(t_ast_node *node, t_minishell *shell)
{
	int	result;

	if (node->type == NODE_COMMAND)
	{
		result = execute_command_node(node, shell);
		return (result);
	}
	else if (node->type == NODE_PIPE)
	{
		init_pipe(node, shell);
		return (0);
	}
	else if (node->type == NODE_AND || node->type == NODE_OR)
	{
		result = execute_logical_operator_node(node, shell);
		return (result);
	}
	return (-1);
}

/*	init cmd struct & exec either external or builtin cmd based on token type */
int	execute_command_node(t_ast_node *node, t_minishell *shell)
{
	t_command	cmd;

	init_command(&cmd, node->tokens, shell);
	if (node->tokens->type == TOKEN_EXTERN_CMD)
		execute_external(shell->cmd, shell->env, shell);
	else if (node->tokens->type == TOKEN_BUILTIN_CMD)
		execute_builtin(shell);
	return (0);
}

/*	evaluate left subtree and, based on the operator type and left result,
	decide whether to eval right subtree; return result of left subtree eval */
int	execute_logical_operator_node(t_ast_node *node, t_minishell *shell)
{
	int	left_result;

	left_result = evaluate_and_execute(node->left, shell);
	if ((node->type == NODE_AND && left_result == 0)
		|| (node->type == NODE_OR && left_result != 0))
	{
		evaluate_and_execute(node->right, shell);
	}
	return (left_result);
}
