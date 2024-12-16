/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:56:17 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/16 17:04:14 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*	reset signal handlers, setup redirections, exec cmd in child process.
	if execve fails, print an error, free garbage, and exit with status 126 */
void	execute_command_child(t_command *cmd, char **env, t_minishell *shell)
{
	t_files		*infile;
	t_files		*outfile;
	struct stat	st;

	reset_signal_handlers(shell);
	infile = get_last_file(cmd->infile);
	outfile = get_last_file(cmd->outfile);
	setup_redirections(infile, outfile, shell);

	/* Perform stat on cmd->path to check file status */
	if (stat(cmd->path, &st) == -1)
	{
		/* File doesn't exist */
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->cmd_name, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		gc_free_all(shell->gc_head);
		exit(127);
	}
	else
	{
		/* File exists, check if it's a directory */
		if (S_ISDIR(st.st_mode))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd->cmd_name, STDERR_FILENO);
			ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
			gc_free_all(shell->gc_head);
			exit(126);
		}

		/* Check if the file is executable */
		if (access(cmd->path, X_OK) != 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd->cmd_name, STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			gc_free_all(shell->gc_head);
			exit(126);
		}
	}

	/* Attempt to execute the command */
	if (execve(cmd->path, cmd->args, env) == -1)
	{
		if (errno == EACCES)
		{
			/* Permission denied */
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd->cmd_name, STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			gc_free_all(shell->gc_head);
			exit(126);
		}
		else if (errno == ENOEXEC)
		{
			/* Exec format error */
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd->cmd_name, STDERR_FILENO);
			ft_putstr_fd(": Exec format error\n", STDERR_FILENO);
			gc_free_all(shell->gc_head);
			exit(126);
		}
		else
		{
			/* Other exec errors */
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd->cmd_name, STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			gc_free_all(shell->gc_head);
			exit(127);
		}
	}
}

/*	wait for child process to finish, update last exit status based on
	child's exit status or termination signal, and setup prompt signals */
void	handle_parent_process(pid_t pid, t_minishell *shell)
{
	int	status;

	ignore_signal_handlers(shell);
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("minishell: waitpid");
		shell->last_exit_status = 1;
	}
	else
	{
		if (WIFEXITED(status))
			shell->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			shell->last_exit_status = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
			else if (WTERMSIG(status) == SIGQUIT)
				ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
		}
		else
			shell->last_exit_status = 1;
	}
	setup_prompt_signals(shell);
}
