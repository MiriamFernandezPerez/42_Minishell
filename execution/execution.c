/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:22:00 by esellier          #+#    #+#             */
/*   Updated: 2024/09/30 21:49:41 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtins_exe(t_data *data, t_section *section)
{
	if (section->fd_in == -1 || section->fd_out == -1)
		exit (1);
	if (section->fd_in != -2)
	{
		if (dup2(section->fd_in, STDIN_FILENO) == -1)
			return (error_exe(data, NULL, 2), data->rt_value);
		close (section->fd_in);
	}
	if (section->fd_out != -2)
	{
		if (dup2(section->fd_out, STDOUT_FILENO) == -1)
			return (error_exe(data, NULL, 2), data->rt_value);
		close (section->fd_out);
	}
	data->rt_value = make_builtins(section->cmd, data);
	ft_free_section(data->sections, NULL);
	return (data->rt_value);
}

int	classic_exe(t_data *data, t_section *section)
{
	if (section->fd_in == -1 || section->fd_out == -1)
		exit (1);
	section->path_array = lst_to_arr(data->env_lst, data, section->path_array);
	if (search_path(data, section->path_array, section) != 0)
		exit (data->rt_value);
	if (section->fd_in != -2)
	{
		if (dup2(section->fd_in, STDIN_FILENO) == -1)
			return (error_exe(data, NULL, 2), data->rt_value);
		close (section->fd_in);
	}
	if (section->fd_out != -2)
	{
		if (dup2(section->fd_out, STDOUT_FILENO) == -1)
			return (error_exe(data, NULL, 2), data->rt_value);
		close (section->fd_out);
		if (section->next && section->next->fd_in != -2)// si c'est un pipe ca ferme la sortie de lecture pour pas aue ca sorte si autre chose ca fait rien car dans l'enfant
			close (section->next->fd_in);
	}
	if (execve(section->path, section->cmd, section->path_array) == -1)
		exit (error_exe(data, "execve", 0));
	return (0);
}


/*int	*ft_waitpid(t_section *section)
{
	int	status;

	waitpid(section->pid, &status, 0);
	return (status);
}

int	ft_waitpid_status(t_section *section)
{
	int	*status;
	int	i;

	while (section)
	{
		status = ft_waitpid(section);
		section = section->next;
		i++;
	}
	if (WIFEXITED(status[i]))
		return (WEXITSTATUS(status[i]));
	if (WIFSIGNALED(status[i]))
		return (WTERMSIGN(status[i]));
}*/

void	free_array_int(int **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free (array[i]);
		i++;
	}
	free(array);
	return ;
}

int	ft_waitpid_status(t_section *section, t_data *data)
{
	int	**status;
	int	i;
	int	tmp;

	i = 0;
	status = (int **)malloc((data->sections_qt + 1) * sizeof (int *));
	while (section)
	{
		status[i] = (int *)malloc(sizeof (int));
		waitpid(section->pid, status[i], 0);
		i++;
		section = section->next;
	}
	status[i] = (int *)malloc(sizeof (int));
	status[i] = NULL;
	tmp = status[i - 1][0];
	free_array_int(status); // leaks avec ctr d pour fermer minishell
	if (WIFEXITED(tmp))
		return (WEXITSTATUS(tmp));
	else if (WIFSIGNALED(tmp))
		return (WTERMSIG(tmp));
	return (0);
}

int	execution(t_data *data, t_section *section)
{
	if (data->sections_qt == 1 && check_builtins(section->cmd) == 0)
		return (builtins_exe(data, section), data->rt_value);
	while (section)
	{
		section->pid = fork();
		if (section->pid < 0)
		{
			perror("Fork error");
			ft_free_data(data);
			exit(1);
		}
		if (section->pid == 0)
		{
			if (check_builtins(section->cmd) == 1)
				data->rt_value = classic_exe(data, section);
			else
				data->rt_value = make_builtins(section->cmd, data);
			exit (data->rt_value);
			// a voir si ok de pas free la data a la fin du hijo
		}
		if (data->rt_value == -1)
		{
			ft_free_data(data);
			exit (1);
		}
		close_fd(section);
		section = section->next;
	}
	data->rt_value = ft_waitpid_status(data->sections, data);
	return (ft_free_section(data->sections, section), data->rt_value);
}
// si deux echo ne devrait pas imprimer le premier (echo Hola | echo aue tal) = aue tal
//mettre ft_malloc aux fonctions de Miriam
// mettre un flag por el ft_free_data
//ajouter les fonctions aue miriam a modifie sur slack
// il y a une boucle et il imprime le spritf de test dans les fichiers, je pense aue je n'ai pas bien fermer un fd