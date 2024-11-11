/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 16:26:15 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/11 16:49:28 by dlemaire         ###   ########.fr       */
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
		if (root->tokens->type == TOKEN_BI_ECHO)
			printf("implement echo\n");
		else
			printf("execute executable\n");
	}
}