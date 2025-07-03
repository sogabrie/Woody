/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sogabrie <sogabrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 20:08:34 by sogabrie          #+#    #+#             */
/*   Updated: 2023/04/26 20:08:35 by sogabrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (f)(unsigned int, char))
{
	unsigned int	i;
	char			*mas;

	i = 0;
	if (!s || !f)
		return (0);
	mas = ft_strdup(s);
	if (!mas)
		return (0);
	while (mas[i])
	{
		mas[i] = f(i, mas[i]);
		++i;
	}
	return (mas);
}
