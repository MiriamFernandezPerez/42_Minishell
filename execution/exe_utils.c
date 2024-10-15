/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:03:19 by esellier          #+#    #+#             */
/*   Updated: 2024/10/15 20:04:28 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_three(char const *s1, char const *s2, char const *s3)
{
	size_t	ls;
	size_t	ls1;
	size_t	ls2;
	char	*m;

	ls = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	ls1 = ft_strlen(s1);
	ls2 = ft_strlen(s1) + ft_strlen(s2);
	if (s1 && s2 && s3)
	{
		m = (char *)malloc(ls + 1);
		if (!m)
			return (NULL);
		ft_strlcpy(m, s1, ls1 + 1);
		ft_strlcat(m, s2, ls2 + 1);
		ft_strlcat(m, s3, ls + 1);
		return (m);
	}
	return (NULL);
}

char	**lst_to_arr(t_env *lst, t_data *data, char **array)
{
	int		i;

	i = 0;
	array = (char **)malloc((count_lst(lst) + 1) * sizeof (char *));
	if (!array)
		ft_malloc(data, NULL, NULL);
	while (lst)
	{
		if (lst->name && lst->value)
		{
			array[i] = ft_strjoin_three(lst->name, "=", lst->value);
			if (!array[i])
				ft_malloc(data, array, NULL);
		}
		if (!lst->value)
		{
			array[i] = ft_strjoin(lst->name, "=");
			if (!array[i])
				ft_malloc(data, array, NULL);
		}
		lst = lst->next;
		i++;
	}
	array[i] = '\0';
	return (array);
}

int	error_exe(t_data *data, char *arg, int i)
{
	if (i != 1)
		write(2, "minishell: ", 11);
	if (i == 2 || i == 3 || i == 4)
		perror(arg);
	if (i == 0 || i == 1 || i == 4)
	{
		if (i == 0 || i == 1)
			write(2, arg, ft_strlen(arg));
		if (i == 0)
			write(2, ": No such file or directory\n", 28);
		if (i == 1)
			write(2, ": command not found\n", 20);
		//if (i == 4)
		//	return (126); return execve int
		return (data->rt_value = 127, 127);
	}
	if (i == 2)
		ft_free_data(data, 1);
	if (i == 3)
	{
		ft_free_data(data, 0);
		erase_lst(data->env_lst);
		exit (-1);
	}
	return (data->rt_value = 1, 1);
}


int	check_builtins(char **str)
{
	if ((ft_strcmp("echo", str[0]) == 0) || (ft_strcmp("cd", str[0]) == 0)
		|| (ft_strcmp("pwd", str[0]) == 0) || (ft_strcmp("export", str[0]) == 0)
		|| (ft_strcmp("unset", str[0]) == 0) || (ft_strcmp("env", str[0]) == 0)
		|| (ft_strcmp("exit", str[0]) == 0))
		return (0);
	else
		return (1);
}

void	exe_builtins_redi(t_data *data, int fd_in, int fd_out)
{
	if (dup2(fd_out, STDOUT_FILENO) == -1 || dup2(fd_in, STDIN_FILENO) == -1)
	{
		close (fd_in);
		close (fd_out);
		error_exe(data, "Dup2 error", 2);
		exit(1);
	}
	close (fd_in);
	close (fd_out);
	return ;
}
//fonction qui remet le stdin et stdout a l'origine car pas de hijo dans les builtins