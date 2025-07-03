/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sogabrie <sogabrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 20:08:04 by sogabrie          #+#    #+#             */
/*   Updated: 2023/04/26 20:08:05 by sogabrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	l;
	char	*ptr;

	l = ft_strlen(s) + 1;
	ptr = (char *)malloc(l);
	if (!ptr)
		return (0);
	ft_strlcpy(ptr, s, l);
	return (ptr);
}
