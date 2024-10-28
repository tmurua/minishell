/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:35:59 by dlemaire          #+#    #+#             */
/*   Updated: 2024/10/28 19:53:00 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_lexer {
    const char  *str;
    size_t      pos;
    char        current_char;
} t_lexer;

int	ft_iswhitespaces(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && 
			str[i] != '\v' && str[i] != '\f' && str[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}

char	*ft_strndup(const char *s, size_t n)
{
	int		index;
	char	*copy;

	copy = malloc((n + 1) * sizeof(char));
	if (!copy)
		return (NULL);
	index = 0;
	while (index <= n)
	{
		copy[index] = s[index];
		index++;
	}
	copy[index] = '\0';
	return (copy);
}

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
    return (token);
}

t_token get_next_token(t_lexer *lexer)
{
    t_token token;
    size_t  start_pos;
    size_t  length;
    char    *arg;

    while (lexer->current_char != '\0' && ft_iswhitespaces(lexer->current_char))
    {
        move_forward(lexer);
    }
    if (ft_strncmp(lexer->str[lexer->pos], "echo", 4) == 0)
    {
        lexer->pos += 4;
        lexer->current_char = lexer->str[lexer->pos];
        return (create_token(BUILTIN_CMD, "echo"));
    }
    if (lexer->current_char == '"')
    {
        move_forward(lexer);
        start_pos = lexer->pos;
        while (lexer->current_char != '"' && lexer->current_char != '\0')
        {
            move_forward(lexer);
        }
        if (lexer->current_char == '"')
        {
            length = lexer->pos - start_pos;
            arg = ft_strndup(lexer->str[start_pos], length);
            move_forward(lexer);
            return (create_token(ARGUMENT, arg));
        }
        else
        {
            printf("Error: unterminated string\n");
            exit(1);           
        }
    }
    return (create_token(INVALID_TOKEN, NULL));
}

void    run_lexer(char *str)
{
    t_lexer lexer;
    t_token token;
    t_token *tokens;
    int i;
    
    lexer = init_lexer(str);
    tokens = malloc(sizeof(t_token) * 3); // needs logic to determine amount of tokens
    i = 0;
    while (i < 3)
    {
        tokens[i] = get_next_token(&lexer);
        if (tokens[i].type == INVALID_TOKEN)
            break;
    }
}