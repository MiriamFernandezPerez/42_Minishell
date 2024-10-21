/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_files.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:17:45 by esellier          #+#    #+#             */
/*   Updated: 2024/10/21 15:38:22 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_heredoc(int *fd, char *del)
{
	char	*line;

	while (1)
	{
		line = NULL;
		line = readline(">");
		if (!line)
		{
			write(2, "bash: warning: here-document delimited ", 40);
			write(2, "by end-of-file (wanted `", 25);
			write(2, del, ft_strlen(del));
			write(2, "')\n", 3);
			line = NULL;
			break ;
		}
		if (ft_strncmp(line, del, ft_strlen(line) + 1) == 0)
		{
			free (line);
			close (fd[1]);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		write (fd[1], "\n", 1);
		free (line);
	}
}

int	ft_heredoc(t_data *data, char *del)
{
	int		fd[2];
	int		pid;
	int		status;

	status = 0;
	if (pipe(fd) == -1)
		error_exe(data, "pipe error", 2);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid < 0)
		error_exe(data, "fork error", 2);
	if (pid == 0)
	{
		close(fd[0]);
		signal(SIGINT, heredoc_sigint_handler);
		do_heredoc(fd, del);
		close(fd[1]);
		exit (0);
	}
	close (fd[1]);
	waitpid(pid, &status, 0);
	if (status != 0)
		return ((data->rt_value = 128 + SIGINT), -3);
	return (fd[0]);
}

int	create_file(char *file, int i, t_data *data, int fd)
{
	if (fd > -1)
		close(fd);
	if (i == INPUT)
		fd = open(file, O_RDONLY);
	else if (i == TRUNC)
		fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else if (i == APPEND)
		fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0644);
	else if (i == HEREDOC)
		fd = ft_heredoc(data, file);
	if (fd == -1)
		error_exe(data, file, 0);
	return (fd);
}

int	fd_null(t_data *data, t_section *section)
{
	int	fd;
	int	fd2;

	if (section->fd_in == -2 && section != data->sections)
	{
		fd = open("/dev/null", O_RDONLY);
		if (fd == -1)
			error_exe(data, "open", 2);
		if (dup2(fd, STDIN_FILENO) == -1)
			return (error_exe(data, "dup2 error", 3));
		close (fd);
	}
	if ((section->fd_out == -2 && data->sections_qt > 1 && section->next)
		|| ft_strcmp(section->cmd[0], "exit") == 0)
	{
		fd2 = open("/dev/null", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd2 == -1)
			error_exe(data, "open", 2);
		if (dup2(fd2, STDOUT_FILENO) == -1)
			return (error_exe(data, "dup2 error", 3));
		close (fd2);
	}
	return (0);
}

int	check_files(t_data *data, t_section *current, t_red *red)
{
	if (data->sections)
		current = data->sections;
	while (current)
	{
		if (current->files)
		{
			red = current->files;
			while (red)
			{
				if (red->redi == INPUT || red->redi == HEREDOC)
					current->fd_in = create_file(red->file, red->redi, data,
							current->fd_in);
				if (current->fd_in == -3)
					return (-3);
				if (red->redi == TRUNC || red->redi == APPEND)
					current->fd_out = create_file(red->file, red->redi, data,
							current->fd_out);
				if (current->fd_in == -1 || current->fd_out == -1)
					break ;
				red = red->next;
			}
		}
		current = current->next;
	}
	return (0);
}
