/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 17:10:48 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/03 20:50:17 by dlemaire         ###   ########.fr       */
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

int is_builtin(const char *cmd)
{
    return (ft_strncmp(cmd, "echo", 4) == 0 || ft_strncmp(cmd, "cd", 2) == 0 ||
            ft_strncmp(cmd, "pwd", 3) == 0 || ft_strncmp(cmd, "export", 6) == 0);
}

void    free_ast_node(t_ast_node *node)
{
    int i;
    
    if (!node)
        return ;
    free(node->command.cmd_name);
    i = 0;
    while (node->command.args[i])
    {
        free(node->command.args[i]);
        i++;
    }
    free(node->command.args);
    free(node);
}
// should be called after command execution (very end) when AST not needed
void    free_ast(t_ast_node *head)
{
    t_ast_node  *temp;

    while (head)
    {
        temp = head;
        head = head->next;
        free_ast_node(temp);
    }
}

int count_tokens_in_arr(t_token *tokens)
{
    // TODO: create end value for token type in lexer and use it in the loop
    
    int count;

    count = 0;
    while (tokens->value != NULL)
        count++;
    return (count);
}

t_ast_node  *create_ast_node(t_node_type type, char *cmd_name, char *args[])
{
    t_ast_node  *node;
    int         arg_amount;
    int         i;

    node = malloc(sizeof(t_ast_node));
    if (!node)
        return (NULL);
    node->type = type;
    node->command.cmd_name = ft_strdup(cmd_name);
    if (!node->command.cmd_name)
    {
        free(node);
        return (NULL);
    }
    arg_amount = 0;
    while (args[arg_amount])
        arg_amount++;
    node->command.args = malloc((arg_amount + 1) * sizeof(char *));
    if (!node->command.args)
    {
        free(node->command.cmd_name);
        free(node);
        return (NULL);
    }
    i = 0;
    while (i < arg_amount)
    {
        node->command.args[i] = ft_strdup(args[i]);
        if (!node->command.args[i])
        {
            while (--i >= 0)
                free(node->command.args[i]);
            free(node->command.args);
            free(node->command.cmd_name);
            free(node);
            return (NULL);
        }
        i++;
    }
    node->command.args[arg_amount] = NULL;
    return (node);
}

t_ast_node  *parse_tokens(t_token *tokens)
{
    t_ast_node  *ast_root;
    t_ast_node  **current;
    char        *cmd_name;
    char        **args;
    int         i;
    t_node_type type;
    int         num_args;

    ast_root = malloc(sizeof(t_ast_node));
    if (!ast_root)
        return (NULL);    
    ft_memset(ast_root, 0, sizeof(t_ast_node)); // will default node->type to NODE_COMMAND(0)
    current = malloc(sizeof(t_ast_node *));
    if (!current)
        return (NULL);
    current[0] = ast_root;
    num_args = count_tokens_in_arr(tokens);
    args = malloc((num_args + 1) * sizeof(char *));
    if (!args)
        return (NULL);
    i = 0;
    while (i < num_args)
    {
        args[i] = NULL;
        i++;
    }
    i = 0;
    while (tokens)
    {
        if (!tokens->value)
            break ;
        if (i == 0)
            cmd_name = tokens->value;
        else
            args[i - 1] = tokens->value;
        tokens = tokens->next;
        i++;
    }
    args[i - 1] = NULL;
    if (is_builtin(cmd_name))
        type = NODE_BUILTIN;
    else
        type = NODE_COMMAND;
    *current = create_ast_node(type, cmd_name, args);
    return (ast_root);
}
