/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sogabrie <sogabrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 20:07:19 by sogabrie          #+#    #+#             */
/*   Updated: 2023/04/26 20:07:20 by sogabrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *a, const void *b, size_t s)
{
	unsigned char		*a1;
	const unsigned char	*b1;

	if (!a && !b)
		return (a);
	if (a < b)
		return (ft_memcpy(a, b, s));
	a1 = (unsigned char *)a;
	b1 = (unsigned char *)b;
	while (s--)
		a1[s] = b1[s];
	return (a);
}
