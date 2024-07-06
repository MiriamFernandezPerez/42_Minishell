/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:59:43 by esellier          #+#    #+#             */
/*   Updated: 2024/07/06 19:40:05 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.c>

int modulo_negativ(int a)
{
    int r;

    while (a < 0)
        a = a + 256;
    r = a % 256;
    return (r);    
}