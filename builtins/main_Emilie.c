/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_Emilie.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:17:55 by esellier          #+#    #+#             */
/*   Updated: 2024/09/09 15:07:24 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	main(int argc, char **argv, char **env)
{
	char	**str;
	t_data	*data;
	int		i;
	int		j;
	//t_env	*current;
	//(void) argc;
	(void) argv;
	
	i = 0;
	j = 1;
	data = NULL;
	str = malloc((argc) * sizeof(char *));
	while (i < argc -1)
		str[i++] = argv[j++];
	str[i] = '\0';
	/*i = 0;
	while (str[i])
	{
		printf ("str = %s\n", str[i]);
		i++;
	}*/
	ft_initialize(&data, env);
	make_echo(str);
	/*current = data->env_lst;
	while (current)
	{
		printf ("%s=%s\n", current->name, current->value);
		current = current->next;
	}*/
	/*current = data->env_lst;
	while (current)
	{
		printf("%s=%s FLAG=%d\n", current->name, current->value, current->flag);
		current = current->next;
	}*/
	final_free(data);
	free(str);
	return (0);
}
//verifier avec un nom _test si dans l'export et l'env

//ATTENTION Miriam envoie les args separes de la cmd, changer les fonctions comme ca