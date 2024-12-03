/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:09:18 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/03 17:49:35 by tmurua           ###   ########.fr       */
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
		return (handle_numeric_argument(args[1]));
	else
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
}

/* handle numeric argument for exit */
int	handle_numeric_argument(char *arg)
{
	if (is_numeric_argument(arg))
		return (ft_atoi(arg) % 256);
	else
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (2);
	}
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
