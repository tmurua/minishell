/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 16:26:15 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/20 16:34:40 by tmurua           ###   ########.fr       */
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
			run_program(&cmd);
		else if (node->tokens->type == TOKEN_BUILTIN_CMD)
			execute_builtin(cmd.args, shell);
		return (0);
	}
	else if (node->type == NODE_PIPE)
	{
		//evaluate_and_execute(node->left, envp);
		//evaluate_and_execute(node->right, envp);
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

/*	helper function to execute a command node;convert tokens to
	args and execute either builtin cmd or external command */
// void	execute_command_node(t_token *tokens, t_minishell *shell)
// {
// 	char	**args;

// 	args = tokens_to_args(tokens);
// 	if (args == NULL)
// 		return ;
// 	if (tokens->type == TOKEN_BUILTIN_CMD)
// 		execute_builtin(args, shell);
// 	else
// 		execute_external_cmd(args, shell, tokens);
// 	free_arguments(args);
// }
