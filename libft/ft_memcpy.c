/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sogabrie <sogabrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 20:07:15 by sogabrie          #+#    #+#             */
/*   Updated: 2023/04/26 20:07:16 by sogabrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *a, const void *b, size_t s)
{
	size_t	i;

	if (!a && !b)
		return (0);
	i = 0;
	while (i < s)
	{
		((unsigned char *)a)[i] = ((unsigned char *)b)[i];
		++i;
	}
	return (a);
}
