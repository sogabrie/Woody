/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sogabrie <sogabrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 20:07:06 by sogabrie          #+#    #+#             */
/*   Updated: 2023/04/26 20:07:07 by sogabrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*a;
	size_t			i;
	unsigned char	c2;

	i = 0;
	c2 = (unsigned char)c;
	a = (unsigned char *) s;
	while (i < n)
	{
		if (*a == c2)
			return (a);
		++a;
		++i;
	}
	return (0);
}
