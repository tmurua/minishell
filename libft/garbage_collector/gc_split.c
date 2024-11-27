/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 11:26:54 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/27 12:33:42 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	gc_count_words(const char *s, char c);
static void	gc_free_arr_strs(char **arr_strs, size_t i);

/*	allocate mem using gc_calloc and track allocations with garbage collector */
char	**gc_split(t_list **gc_head, char const *s, char c)
{
	size_t	i;
	char	**arr_strs;
	int		word_len;

	if (!s)
		return (NULL);
	i = 0;
	arr_strs = gc_calloc(gc_head, gc_count_words(s, c) + 1, sizeof(char *));
	if (!arr_strs)
		return (NULL);
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			if (!ft_strchr(s, c))
				word_len = ft_strlen(s);
			else
				word_len = ft_strchr(s, c) - s;
			arr_strs[i] = gc_substr(gc_head, s, 0, word_len);
			if (arr_strs[i] == NULL)
			{
				gc_free_arr_strs(arr_strs, i);
				return (NULL);
			}
			i++;
			s += word_len;
		}
	}
	arr_strs[i] = NULL;
	return (arr_strs);
}

/* count number of words in string 's' separated by delimiter 'c' */
static int	gc_count_words(const char *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s && *s != c)
			s++;
	}
	return (count);
}

/*	free array of strings up to index 'i' in case of an allocation failure */
static void	gc_free_arr_strs(char **arr_strs, size_t i)
{
	while (i > 0)
	{
		i--;
		if (arr_strs[i])
			free(arr_strs[i]);
	}
	free(arr_strs);
}
