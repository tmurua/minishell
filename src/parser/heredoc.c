/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 23:18:42 by dlemaire          #+#    #+#             */
/*   Updated: 2024/12/04 18:54:16 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	heredoc_loop(t_minishell *shell, t_token *token, int *pipe,
			t_files	*heredoc);
int		is_heredoc_delimiter(const char *input, const char *delimiter);
void	catch_heredoc_input(t_minishell *shell, char *str, int fd,
			t_files *heredoc);
t_files	*save_heredoc_fd(t_list *heredoc_list, int new_fd);
char	*get_env_variable(const char *var_name, char **env);
int		expend_in_heredoc(t_minishell *shell, char *str, int fd);


void	init_heredoc(t_minishell *shell, t_token *token)
{
	int		fd[2];
	pid_t	pid;
	int		status;
	t_files	*heredoc;

	if (pipe(fd) < 0)
		return ;
	pid = fork();
	heredoc = save_heredoc_fd(shell->heredocs, fd[0]);
	if (pid == 0)
		heredoc_loop(shell, token, fd, heredoc);
	if (waitpid(pid, &status, 0) == -1)
		perror("minishell: waitpid");
	close(fd[1]);
}

void	heredoc_loop(t_minishell *shell, t_token *token, int *pipe,
			t_files	*heredoc)
{
	char	*buffer;
	char	*delimiter;

	buffer = NULL;
	delimiter = token->next->value;
	close(pipe[0]);
	while (1)
	{
		buffer = readline("> ");
		if (!buffer || is_heredoc_delimiter(buffer, delimiter))
			break ;
		catch_heredoc_input(shell, buffer, pipe[1], heredoc);
	}
	close(pipe[1]);
	//close_heredoc (??)
	rl_clear_history();
	exit(0);
}

int	is_heredoc_delimiter(const char *input, const char *delimiter)
{
	size_t	delimiter_len;

	delimiter_len = ft_strlen(delimiter);
	if (ft_strncmp(input, delimiter, delimiter_len) == 0)
	{
		if (input[delimiter_len] == '\0' || input[delimiter_len] == '\n')
			return (1);
	}
	return (0);
}

void	catch_heredoc_input(t_minishell *shell, char *str, int fd,
			t_files *heredoc)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && !heredoc->heredoc_quote)
			i += expend_in_heredoc(shell, str + i, fd);
		else
		{
			ft_putchar_fd(str[i], fd);
			i++;
		}
	}
	ft_putchar_fd('\n', fd); // unnecessary? 
}

t_files	*save_heredoc_fd(t_list *heredoc_list, int new_fd)
{
	t_list	*outer;
	t_files	*inner;

	if (!heredoc_list)
		return (NULL);
	outer = heredoc_list;
	while (outer)
	{
		inner = (t_files *)outer->content;
		while (inner)
		{
			if (inner->fd == -1)
			{
				inner->fd = new_fd;
				return (inner);
			}
			inner = inner->next;
		}
		outer = outer->next;
	}
	return (NULL);
}

char	*get_env_variable(const char *var_name, char **env)
{
	size_t	name_len;
	char	*current_var;

	if (!var_name || !env)
		return (NULL);
	name_len = ft_strlen(var_name);
	while (*env)
	{
		current_var = *env;
		if (ft_strncmp(current_var, var_name, name_len) == 0
			&& current_var[name_len] == '=')
			return (current_var + name_len + 1);
		env++;
	}
	return (NULL);
}

int	expend_in_heredoc(t_minishell *shell, char *str, int fd)
{
	int		length;
	char	save;
	char	*expanded_var;

	length = 1;
	while (str[length] && str[length] != '$' && !ft_iswhitespace(str[length]))
		length++;
	save = str[length];
	str[length] = '\0';
	expanded_var = get_env_variable(str + 1, shell->env);
	if (expanded_var)
		ft_putstr_fd(expanded_var, fd);
	str[length] = save;
	return (length);
}

void	close_heredoc_list(t_minishell *shell)
{
	t_files	*tmp;
	t_files	*buf;
	t_list	*old_head;
	t_list	*last;

	if (!shell->heredocs || !shell->heredocs->content)
		return ;
	tmp = (t_files *)(shell->heredocs->content);
	while (tmp)
	{
		buf = tmp->next;
		close(tmp->fd);
		tmp = buf;
	}
	old_head = shell->heredocs;
	last = ft_lstlast(shell->heredocs);
	shell->heredocs = shell->heredocs->next;
	last->next = old_head;
	old_head->next = NULL;
}

