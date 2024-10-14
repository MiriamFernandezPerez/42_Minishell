/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 22:07:08 by mirifern          #+#    #+#             */
/*   Updated: 2024/10/09 20:26:52 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Global Variable Initzialize
int	g_signal_num = 0;

/*Función de prueba para imprimir las secciones | comentar o eliminar al final*/
/*void	print_sections(t_data *data)
{
	t_section	*current_section;
	int			j;
	int			section_num;

	if (!data->sections)
		return ;
	section_num = 0;
	current_section = data->sections;
	printf("data->sections_qt: %d\n", data->sections_qt);
	while (current_section)
	{
		printf("SECCION %d:\n", section_num + 1);
		if (!current_section->cmd)
			printf("  No hay comandos en esta sección\n");
		else
		{
			j = 0;
			while (current_section->cmd && current_section->cmd[j])
			{
				printf("  Comando %d: %s\n", j + 1, current_section->cmd[j]);
				j++;
			}
			if (current_section->files)
				printf("  Redir tipo %d, nombre archivo %s\n",
					current_section->files->redi, current_section->files->file);
			else
				printf("Sin redirecciones\n");
		}
		current_section = current_section->next;
		section_num++;
		if (section_num >= data->sections_qt)
			break ;
	}
}*/

//Write Tokens array
void	print_tokens(t_data *data)
{
	int		i;

	i = 0;
	printf("QT TOKENS = %d\n", data->tokens_qt);
	while (i < data->tokens_qt)
	{
		ft_putstr_fd("Token ", 1);
		ft_putnbr_fd(i, 1);
		write(1, " ", 1);
		ft_putstr_fd(data->tokens[i]->value, 1);
		ft_putstr_fd(" * Type:  ", 1);
		ft_putnbr_fd(data->tokens[i]->type, 1);
		write(1, "\n", 1);
		i++;
	}
}

/*Funcion que inicializa la estructura data
y hace una copia del environment*/
int	ft_initialize(t_data **data, char **env)
{
	*data = malloc(sizeof(t_data));
	if (!*data)
	{
		write(2, "malloc error, please check your computer's memory\n", 50);
		exit(EXIT_FAILURE);
	}
	(*data)->prompt = NULL;
	(*data)->tokens = NULL;
	(*data)->sections = NULL;
	(*data)->tokens_qt = 0;
	(*data)->sections_qt = 0;
	(*data)->rt_value = 0;
	(*data)->env_lst = create_env(env, (*data));
	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;
	int		prompt;

	prompt = 0;
	(void)av;
	data = NULL;
	if (ac != 1)
		return (ft_msn(NO_ARGS, 2));
	ft_initialize(&data, env);
	while (1)
	{
		signal(SIGQUIT, readline_sigquit_handler);
		signal(SIGINT, readline_sigint_handler);
		prompt = ft_read_prompt(data);
		if (prompt == -1)
			break ;
		if (prompt == 0)
		{
			signal(SIGINT, SIG_IGN);
			//printf ("prompt = %d \n", prompt);
			/*if (data->sections->cmd)
				printf ( "cmd = %s \n", data->sections->cmd[0]);
			if (data-> sections->files)
				printf ("file_main1 = %s\n", data->sections->files->file);
			if (data-> sections->files->next)
				printf ("file_main2 = %s\n", data->sections->files->next->file);*/
			check_files(data, data->sections, NULL);
			create_pipe(data);
			signal(SIGQUIT, exe_sigquit_handler);
			signal(SIGINT, exe_sigint_handler);
			execution(data, data->sections);
		}
		ft_free_data(data, 0);
	}
	if (data)
		ft_free_data(data, 1);
	return (0);
}