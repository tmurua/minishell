/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 21:29:49 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/13 21:30:04 by tmurua           ###   ########.fr       */
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

	start = *str;
	while (**str && !ft_iswhitespace(**str) && **str != '"' && **str != '\'')
		(*str)++;
	len = *str - start;
	return (gc_substr(gc_head, start, 0, len));
}
