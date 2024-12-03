/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:09:18 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/03 16:15:09 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int handle_exit_command(char *input, t_minishell *shell)
{
	char	**args;
	int		arg_count;
	int		exit_status;

	if (input == NULL || ft_strncmp(input, "exit", 4) != 0)
		return (0);
	args = gc_split(&shell->gc_head, input, ' ');
	if (!args)
		return (0);
	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	printf("exit\n");
	if (arg_count == 1)
		exit_status = shell->last_exit_status;
	else if (arg_count == 2)
	{
		if (is_numeric_argument(args[1]))
			exit_status = ft_atoi(args[1]) % 256;
		else
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit_status = 2;
		}
	}
	else
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	rl_clear_history();
	gc_free_all(shell->gc_head);
	exit(exit_status);
	return (0);
}

// Helper function to check if a string represents a numeric argument
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
