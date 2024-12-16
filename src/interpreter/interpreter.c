/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 16:26:15 by dlemaire          #+#    #+#             */
/*   Updated: 2024/12/17 00:25:46 by dlemaire         ###   ########.fr       */
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
void	evaluate_and_execute(t_ast_node *node, t_minishell *shell)
{
	if (node->type == NODE_COMMAND)
		execute_command_node(node, shell);
	else if (node->type == NODE_PIPE)
		init_pipe(node, shell);
	else if (node->type == NODE_AND || node->type == NODE_OR)
		execute_logical_operator_node(node, shell);
}

/*	init cmd struct & exec either external or builtin cmd based on token type */
void	execute_command_node(t_ast_node *node, t_minishell *shell)
{
	t_command	cmd;

	init_command(&cmd, node->tokens, shell);
	while (node->tokens && cmd.redirect_error_flag != 1)
	{
		if (node->tokens && node->tokens->type == TOKEN_EXTERN_CMD)
			execute_external(&cmd, shell->env, shell);
		else if (node->tokens && node->tokens->type == TOKEN_BUILTIN_CMD)
			execute_builtin(&cmd, shell);
		node->tokens = node->tokens->next; // leak?
	}
}

/*	evaluate left subtree and, based on the operator type and left result,
	decide whether to eval right subtree; return result of left subtree eval */
void	execute_logical_operator_node(t_ast_node *node, t_minishell *shell)
{
	evaluate_and_execute(node->left, shell);
	if ((node->type == NODE_AND && shell->last_exit_status == 0)
		|| (node->type == NODE_OR && shell->last_exit_status != 0))
		evaluate_and_execute(node->right, shell);
}
