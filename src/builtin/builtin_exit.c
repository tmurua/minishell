/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:09:18 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/10 21:14:22 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* main function to handle exit command */
int	handle_exit_command(char *input, t_minishell *shell)
{
	char	**args;
	int		exit_status;

	if (ft_strncmp(input, "exit", 4) != 0)
		return (0);
	args = gc_split(&shell->gc_head, input, ' ');
	if (!args)
		return (0);
	printf("exit\n");
	exit_status = handle_exit_with_arguments(args, shell);
	rl_clear_history();
	gc_free_all(shell->gc_head);
	exit(exit_status);
	return (0);
}

/* handle exit command with arguments */
int	handle_exit_with_arguments(char **args, t_minishell *shell)
{
	int	arg_count;

	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	if (arg_count == 1)
		return (shell->last_exit_status);
	else if (arg_count == 2)
		return (handle_numeric_argument(args[1], shell));
	else
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
}

/* handle numeric argument for exit */
int	handle_numeric_argument(char *arg, t_minishell *shell)
{
	char	*cleaned_arg;

	cleaned_arg = remove_surrounding_quotes(arg, shell);
	if (!cleaned_arg)
		return (2);
	if (is_numeric_argument(cleaned_arg))
		return (ft_atoi(cleaned_arg) % 256);
	else
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (2);
	}
}

char	*remove_surrounding_quotes(char *arg, t_minishell *shell)
{
	size_t	len;
	char	first_char;

	if (!arg)
		return (NULL);
	len = ft_strlen(arg);
	if (len > 1)
	{
		first_char = arg[0];
		if ((first_char == '\'' || first_char == '\"')
			&& arg[len - 1] == first_char)
			return (gc_substr(&shell->gc_head, arg, 1, len - 2));
	}
	return (gc_strdup(&shell->gc_head, arg));
}

/* check if a string represents a numeric argument */
int	is_numeric_argument(const char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '-' || arg[0] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}
