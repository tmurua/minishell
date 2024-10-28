/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 09:08:10 by tmurua            #+#    #+#             */
/*   Updated: 2024/10/28 10:56:56 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* libraries */
# include "./libft/libft.h"
/* printf */
# include <stdio.h>
/* malloc, free, exit, getenv */
# include <stdlib.h>
/* readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay */
# include <readline/readline.h>
/* add_history */
# include <readline/history.h>
/* strerror, perror */
# include <string.h>
/* write, access, close, dup, dup2, pipe, unlink, read */
# include <unistd.h>  // isatty, ttyname, ttyslot, getcwd, chdir
/* open */
# include <fcntl.h>
/* opendir, readdir, closedir */
# include <dirent.h>
/* fork, wait, waitpid, wait3, wait4, execve */
# include <sys/types.h>
/* wait, waitpid, wait3, wait4 */
# include <sys/wait.h>
/* stat, lstat, fstat */
# include <sys/stat.h>
/* ioctl */
# include <sys/ioctl.h>
/* signal, sigaction, sigemptyset, sigaddset, kill */
# include <signal.h>
/* tcsetattr, tcgetattr */
# include <termios.h>
/* tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs */
# include <curses.h>
/* perror */
# include <errno.h>    // errno

/* open,close */

/* macros*/

/* structures */

/* function prototypes */

#endif
