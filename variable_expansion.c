/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:33:39 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/08 19:28:02 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	advance_lexer_char to skip $; collect_variable_name; get_variable_value;
	if variable doesn't exist, use an empty str, ""; append value str to buffer;
	return status, 1 on sucess or 0 on failure */
int	handle_variable_expansion(t_lexer *lexer, char **buffer)
{
	char	*var_name;
	char	*var_value;
	int		status;

	advance_lexer_char(lexer);
	var_name = collect_variable_name(lexer);
	if (!var_name)
		return (0);
	var_value = get_variable_value(var_name);
	free(var_name);
	if (!var_value)
		var_value = "";
	status = append_to_buffer(buffer, var_value);
	return (status);
}

/*	current_pos after $ to start collecting var_name; update lexer position,
	i.e. move past the variable name */
char	*collect_variable_name(t_lexer *lexer)
{
	const char	*current_pos;
	int			name_length;
	char		*var_name;

	current_pos = lexer->str + lexer->pos;
	name_length = get_variable_name_length(current_pos);
	if (name_length == 0)
		return (ft_strdup(""));
	var_name = malloc(name_length + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, current_pos, name_length + 1);
	lexer->pos += name_length;
	if (lexer->pos < ft_strlen(lexer->str))
		lexer->current_char = lexer->str[lexer->pos];
	else
		lexer->current_char = '\0';
	return (var_name);
}

/* loop chars after $, if char is alphanumeric or _, it forms variable name */
int	get_variable_name_length(const char *str)
{
	int	length;

	length = 0;
	while (str[length] && (ft_isalnum(str[length]) || str[length] == '_'))
		length++;
	return (length);
}

/* getenv return value of env var based on its NAME; if non existent return ""*/
char	*get_variable_value(const char *var_name)
{
	char	*var_value;

	var_value = getenv(var_name);
	if (var_value)
		return (var_value);
	else
		return ("");
}

/*	join new str to existing buffer; free old buffer; update to new;
	return 1 on success and 0 on failure */
int	append_to_buffer(char **buffer, const char *str)
{
	char	*new_buffer;

	new_buffer = ft_strjoin(*buffer, str);
	if (!new_buffer)
	{
		free(*buffer);
		*buffer = NULL;
		return (0);
	}
	free(*buffer);
	*buffer = new_buffer;
	return (1);
}
