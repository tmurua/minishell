/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:19:43 by dlemaire          #+#    #+#             */
/*   Updated: 2024/10/28 16:05:12 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*parse_arguments(void)
{
	char	*input;

	input = readline("minishell$> ");
	return (input);
}

int	main(int ac, char *av[], char *envp[])
{
	char	*input;

	if (ac != 1 || av[1])
	{
		printf("minishell does not accept arguments\n");
		exit(0);
	}
	while (1)
	{
		input = parse_arguments();
		printf("you entered: %s\n", input);
		free(input);
	}
	(void) envp;
	return (0);
}
