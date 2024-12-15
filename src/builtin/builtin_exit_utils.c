/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 21:29:49 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/15 19:05:47 by tmurua           ###   ########.fr       */
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
	char	*arg;

	arg = read_initial_unquoted_part(gc_head, str);
	if (!arg)
		return (NULL);
	if ((arg[0] == '+' || arg[0] == '-') && (**str == '"' || **str == '\''))
		arg = append_quoted_part(gc_head, str, arg);
	return (arg);
}

char	*read_initial_unquoted_part(t_list **gc_head, const char **str)
{
	const char	*start;
	size_t		len;

	start = *str;
	while (**str && !ft_iswhitespace(**str) && **str != '"' && **str != '\'')
		(*str)++;
	len = *str - start;
	return (gc_substr(gc_head, start, 0, len));
}

char	*append_quoted_part(t_list **gc_head, const char **str, char *arg)
{
	char		quote;
	const char	*qstart;
	size_t		len;
	char		*quoted_part;
	char		*joined;

	quote = **str;
	(*str)++;
	qstart = *str;
	while (**str && **str != quote)
		(*str)++;
	len = *str - qstart;
	if (**str == quote)
		(*str)++;
	quoted_part = gc_substr(gc_head, qstart, 0, len);
	if (!quoted_part)
		return (arg);
	joined = gc_strjoin(gc_head, arg, quoted_part);
	return (joined);
}
