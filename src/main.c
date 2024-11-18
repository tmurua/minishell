/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:19:43 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/18 18:39:27 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int ac, char *av[], char *envp[])
{
	char	**env;

	setup_prompt_signals();
	env = duplicate_env(envp);
	if (!env)
	{
		perror("minishell: duplicate_env");
		return (1);
	}
	main_input_loop(ac, av, env);
	free_env(env);
	return (0);
}
