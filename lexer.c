/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:35:59 by dlemaire          #+#    #+#             */
/*   Updated: 2024/10/29 14:41:28 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// A lexical analyzer reads characters from the input and groups them 
// into token objects
// -> skip whitespaces
// -> recognise keywords
// -> create tokens

#include "minishell.h"

t_lexer init_lexer(const char *arg)
{
    t_lexer lexer;

    lexer.str = arg;
    lexer.pos = 0;
    lexer.current_char = arg[0];
    return (lexer);
}

void    move_forward(t_lexer *lexer)
{
    lexer->pos++;
    if (lexer->pos < ft_strlen(lexer->str))
        lexer->current_char = lexer->str[lexer->pos];
    else
        lexer->current_char = '\0';
}

t_token create_token(int type, char *value)
{
    t_token token;

    token.type = type;
    token.value = value;
    token.next = NULL;
    return (token);
}

t_token get_next_token(t_lexer *lexer)
{
    size_t  start_pos;
    size_t  length;
    char    *arg;

    while (lexer->current_char != '\0' && ft_iswhitespace(lexer->current_char))
    {
        move_forward(lexer);
    }
    if (ft_strncmp(&lexer->str[lexer->pos], "echo", 4) == 0)
    {
        lexer->pos += 4;
        lexer->current_char = lexer->str[lexer->pos];
        return (create_token(BUILTIN_CMD, "echo"));
    }
    if (ft_isalnum(lexer->current_char))
    {
        start_pos = lexer->pos;
        while (ft_isalnum(lexer->current_char))
        {
            move_forward(lexer);
        }
        length = lexer->pos - start_pos;
        arg = ft_strndup(&lexer->str[start_pos], length);
        return (create_token(ARGUMENT, arg));
    }
    return (create_token(INVALID_TOKEN, NULL));
}

int count_tokens(const char *str)
{
    int count = 0;
    int in_token_flag = 0;

    while (*str != '\0')
    {
        if (!ft_iswhitespace(*str) && !in_token_flag)
        {
            in_token_flag = 1;
            count++;
        }
        else if (ft_iswhitespace(*str))
        {
            in_token_flag = 0;
        }
        str++;
    }
    return count;
}

void    run_lexer(char *str)
{
    t_lexer lexer;
    t_token *tokens;
    int     token_amount;
    int     i;
    
    lexer = init_lexer(str);
    token_amount = count_tokens(str);
    tokens = malloc(sizeof(t_token) * token_amount);
    i = 0;
    while (i < token_amount)
    {
        tokens[i] = get_next_token(&lexer);
        if (tokens[i].type == INVALID_TOKEN)
            break;
        i++;
    }
    // what are we doing with this?
}