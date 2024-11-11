/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 16:26:15 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/11 18:38:39 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// traverses the AST starting from root and following the hierarchy to execute
// the commands in the correct order

#include "minishell.h"

void	read_tree(t_ast_node *root)
{
	if (root->type == NODE_PIPE)
	{
		//add pipex
		printf("TODO: execute pipe logic from pipex\n");
	}
	else if (root->type == NODE_COMMAND)
	{
		if (root->tokens->type == TOKEN_BUILTIN_CMD)
			printf("implement echo\n");
		else
			printf("execute executable\n");
	}
}
