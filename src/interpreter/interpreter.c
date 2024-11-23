/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 16:26:15 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/23 18:45:33 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// traverses the AST starting from root and following the hierarchy to execute
// the commands in the correct order

#include "../../include/minishell.h"

/* main function to read and execute the parsed AST */
void	read_tree(t_ast_node *root, t_minishell *shell)
{
	if (root == NULL)
		return ;
	evaluate_and_execute(root, shell);
}

/*	helper function to evaluate and execute the AST; handle the execution of
	different types of AST nodes (commands, Pipes, logical operators) */
int	evaluate_and_execute(t_ast_node *node, t_minishell *shell)
{
	int			left_result;
	t_command	cmd;

	if (node->type == NODE_COMMAND)
	{
		init_command(&cmd, node->tokens, shell);
		if (node->tokens->type == TOKEN_EXTERN_CMD)
			execute_external(shell->cmd, shell->env);
		else if (node->tokens->type == TOKEN_BUILTIN_CMD)
			execute_builtin(shell);
		return (0);
	}
	else if (node->type == NODE_PIPE)
	{
		init_pipe(node, shell);
		return (0);
	}
	else if (node->type == NODE_AND || node->type == NODE_OR)
	{
		left_result = evaluate_and_execute(node->left, shell);
		if ((node->type == NODE_AND && left_result == 0)
			|| (node->type == NODE_OR && left_result != 0))
			evaluate_and_execute(node->right, shell);
		return (left_result);
	}
	return (-1);
}
