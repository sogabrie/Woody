/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sogabrie <sogabrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 20:08:52 by sogabrie          #+#    #+#             */
/*   Updated: 2023/04/26 20:08:53 by sogabrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*a;
	char	*a1;

	a = (char *) s;
	a1 = (char *) 0;
	while (*a != 0)
	{
		if (*a == (unsigned char)c)
			a1 = a;
		++a;
	}
	if ((unsigned char)c == 0)
		a1 = a;
	return (a1);
}
