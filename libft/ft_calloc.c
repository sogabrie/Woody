/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sogabrie <sogabrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 20:06:02 by sogabrie          #+#    #+#             */
/*   Updated: 2023/04/26 20:06:03 by sogabrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*pt;

	if (nmemb == SIZE_MAX || size == SIZE_MAX)
		return (0);
	pt = malloc(nmemb * size);
	if (!pt)
		return (0);
	ft_bzero(pt, nmemb * size);
	return (pt);
}
