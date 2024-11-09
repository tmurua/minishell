/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 17:10:48 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/09 19:36:01 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Parser reads tokens and creates AST nodes (like a converter) and link them
// with hierarchical realtionship (build the tree)

// AST: hierarchical syntactic structure of the source program

// Any programming construct can be
// handled by making up an operator for the construct and treating as operands
// the semantically meaningful components of that construct

// In an abstract syntax tree for an expression, each
// interior node (vs.leaves) represents an operator. There position dictates
// the precedence

#include "minishell.h"

void	eat_token(t_token **current_token)
{
	t_token	*tmp;

	tmp = *current_token;
	*current_token = (*current_token)->next;
	free(tmp->value);
	free(tmp);
}

int	get_precedence_lvl(int type)
{
	if (type == TOKEN_OR || type == TOKEN_AND)
		return (0);
	return (1);
}

int	is_statement_delimiter(int type)
{
	if (type == TOKEN_OR || type == TOKEN_AND || type == TOKEN_PIPE)
		return (1);
	return (0);
}

t_ast_node	*create_ast_node(t_node_type type, t_ast_node *left,
	t_ast_node *right)
{
	t_ast_node	*new;

	new = malloc(sizeof(t_ast_node));
	new->tokens = NULL;
	new->type = type;
	new->left = left;
	new->right = right;
	return (new);
}

t_ast_node	*parse_command(t_token **current_token)
{
	t_ast_node	*node;
	t_token		*tmp;

	node = create_ast_node(NODE_COMMAND, NULL, NULL);
	while (*current_token && !is_statement_delimiter((*current_token)->type))
	{
		tmp = *current_token;
		*current_token = (*current_token)->next;
		tmp->next = NULL;
		ft_lstadd_back(&node->tokens, tmp);
	}
	return (node);
}

t_ast_node	*parse_expression(t_token **current_token, int precedence_threshold)
{
	t_ast_node	*left;
	t_ast_node	*right;
	int			precedence_lvl;
	int			delimiter;

	left = is_leaf(current_token);
	if (!left)
		return (NULL);
	while (*current_token && is_statement_delimiter((*current_token)->type)
		&& get_precedence_lvl((*current_token)->type) >= precedence_threshold)
	{
		delimiter = (*current_token)->type;
		precedence_lvl = get_precedence_lvl(delimiter) + 1;
		eat_token(current_token);
		right = is_branch(current_token, precedence_lvl);
		if (!right)
			break ;
		left = create_ast_node(delimiter, left, right);
	}
	return (left);
}
