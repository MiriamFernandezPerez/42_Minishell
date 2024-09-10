/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 19:58:06 by esellier          #+#    #+#             */
/*   Updated: 2024/01/18 11:16:10 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>
//#include <string.h>

char	*ft_strchr(const char *s, int c)
{
	int				i;
	unsigned char	c_char;
	unsigned char	*s_char;

	c_char = (unsigned char) c;
	s_char = (unsigned char *) s;
	i = 0;
	while ((s_char[i] != '\0') && (s_char[i] != c_char))
		i++;
	if (s_char[i] == c_char)
		return ((char *)(s_char + i));
	return (0);
}

/*int	main ()
{
	char	s[] = "hello world!";
	char	*r;

	r = ft_strchr(s, 'o');
	printf("%s\n", r);

	r = strchr(s, 'o');
	printf("%s", r);
}*/
