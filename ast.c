/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 17:10:48 by dlemaire          #+#    #+#             */
/*   Updated: 2024/10/29 20:14:01 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// AST: hierarchical syntactic structure of the source program

// Any programming construct can be
// handled by making up an operator for the construct and treating as operands
// the semantically meaningful components of that construct

// In an abstract syntax tree for an expression, each
// interior node represents an operator. 

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

t_ast_node  *create_ast_node(t_node_type type, char *cmd_name, char *args[])
{
    t_ast_node  *node;
    int         arg_amount;
    int         i;

    node = NULL;
    node->type = type;
    node->command.cmd_name = strdup(cmd_name);
    arg_amount = 0;
    while (args[arg_amount])
        arg_amount++;
    node->command.args = malloc((arg_amount + 1) * sizeof(char *));
    i = 0;
    while (i < arg_amount)
    {
        node->command.args[i] = strdup(args[i]);
        i++;
    }
    node->command.args[arg_amount] = NULL;
    return (node);
}

t_ast_node  *parse_tokens(t_token *tokens)
{
    t_ast_node  *ast_head;
    t_ast_node  **current;
    char        *cmd_name;
    char        *args[3]; // how to set this dynamically?
    int         i;
    t_node_type type;

    ast_head = NULL;
    current[0] = ast_head;
    while (tokens)
    {
        if (!tokens->value)
            break ;
        cmd_name = tokens->value;
        tokens = tokens->next;
    
        i = 0;
        args[i++] = tokens->value;
        tokens = tokens->next;
        if (is_builtin(cmd_name))
            type = NODE_BUILTIN;
        else
            type = NODE_COMMAND;
        *current = create_ast_node(type, cmd_name, args);
        current = &(*current)->next;
    }
    return (ast_head);
}
