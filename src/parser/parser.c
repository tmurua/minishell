/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 17:10:48 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/28 16:04:31 by tmurua           ###   ########.fr       */
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

#include "../../include/minishell.h"

void	ft_tkadd_back(t_token **tokens, t_token *new)
{
	t_token	*tmp;

	if (tokens)
	{
		tmp = *tokens;
		if (*tokens == NULL)
			*tokens = new;
		else
		{
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
	}
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
		t_ast_node *right, t_minishell *shell)
{
	t_ast_node	*new;

	new = gc_calloc(&shell->gc_head, 1, sizeof(t_ast_node));
	if (!new)
	{
		perror("minishell: create_ast_node");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	new->tokens = NULL;
	new->type = type;
	new->left = left;
	new->right = right;
	return (new);
}

t_ast_node	*parse_command(t_minishell *shell)
{
	t_ast_node	*node;
	t_token		*tmp;
	int			heredoc_flag;

	heredoc_flag = 0;
	node = create_ast_node(NODE_COMMAND, NULL, NULL, shell);
	while (shell->tokens && !is_statement_delimiter(shell->tokens->type))
	{
		if (shell->tokens->type == TOKEN_HEREDOC)
		{
			init_heredoc(shell, shell->tokens, heredoc_flag);
			heredoc_flag = 1;
		}
		tmp = shell->tokens;
		shell->tokens = shell->tokens->next;
		tmp->next = NULL;
		ft_tkadd_back(&node->tokens, tmp);
	}
	return (node);
}

t_ast_node	*parse_expression(t_minishell *shell, int precedence_threshold)
{
	t_ast_node	*left;
	t_ast_node	*right;
	int			precedence_lvl;
	int			delimiter;

	left = parse_command(shell);
	if (!left)
		return (NULL);
	while (shell->tokens && is_statement_delimiter(shell->tokens->type)
		&& get_precedence_lvl(shell->tokens->type) >= precedence_threshold)
	{
		delimiter = shell->tokens->type;
		precedence_lvl = get_precedence_lvl(delimiter) + 1;
		shell->tokens = shell->tokens->next;
		right = parse_expression(shell, precedence_lvl);
		if (!right)
			break ;
		if (delimiter == TOKEN_PIPE)
			left = create_ast_node(NODE_PIPE, left, right, shell);
		else if (delimiter == TOKEN_AND)
			left = create_ast_node(NODE_AND, left, right, shell);
		else if (delimiter == TOKEN_OR)
			left = create_ast_node(NODE_OR, left, right, shell);
	}
	return (left);
}
