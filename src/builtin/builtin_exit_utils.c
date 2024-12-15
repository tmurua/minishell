/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 21:29:49 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/15 18:50:19 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	skip_spaces(const char **str)
{
	while (**str && ft_iswhitespace(**str))
		(*str)++;
}

char	*read_quoted_arg(t_list **gc_head, const char **str, char quote)
{
	const char	*start;
	size_t		len;

	(*str)++;
	start = *str;
	while (**str && **str != quote)
		(*str)++;
	len = *str - start;
	if (**str == quote)
		(*str)++;
	return (gc_substr(gc_head, start, 0, len));
}

char	*read_unquoted_arg(t_list **gc_head, const char **str)
{
	const char	*start;
	size_t		len;
	char		*arg;
	start = *str;

	while (**str && !ft_iswhitespace(**str) && **str != '"' && **str != '\'')
		(*str)++;
	len = *str - start;
	arg = gc_substr(gc_head, start, 0, len);
	if (!arg)
		return (NULL);
	if ((arg[0] == '+' || arg[0] == '-') && (**str == '"' || **str == '\''))
	{
		char	quote = **str;
		(*str)++;
		const char *qstart = *str;
		while (**str && **str != quote)
			(*str)++;
		len = *str - qstart;
		if (**str == quote)
			(*str)++;
		char *quoted_part = gc_substr(gc_head, qstart, 0, len);
		if (!quoted_part)
			return (arg);
		char *joined = gc_strjoin(gc_head, arg, quoted_part);
		arg = joined;
	}
	return (arg);
}

