/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sogabrie <sogabrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 20:07:43 by sogabrie          #+#    #+#             */
/*   Updated: 2023/04/26 20:07:44 by sogabrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	coufri(char const *s, char c, char **ptr, size_t *i)
{
	int	j;

	j = 0;
	if (*i == 5)
	{
		j = 0;
		while (ptr)
			while (ptr[j])
				free(ptr[j++]);
		free(ptr);
		return ;
	}		
	*i = 0;
	if (s[j--] != c)
		++(*i);
	while (s[++j])
		if (s[j] == c && s[j - 1] != c)
			++(*i);
	if (s[j -1] != c)
		++(*i);
}

size_t	mal(char **ptr, char *s, char *s2, size_t j)
{
	size_t	i;

	i = s2 - s;
	ptr[j] = malloc((i + 1) * sizeof(char));
	if (!ptr[j])
		return (5);
	ft_strlcpy(ptr[j], s, i + 1);
	if (!ptr[j])
		return (5);
	return (0);
}

size_t	mas2(char **ptr, char *s, char c, char *s2)
{
	size_t	j;

	j = 0;
	while (*s)
	{
		if ((*s2 == c || *s2 == 0) && *(s2 - 1) != c && !(s2 == s))
		{
			if (mal(ptr, s, s2, j))
				return (5);
			if (!*s2)
				return (0);
			s = ++s2;
			++j;
		}
		else
		{
			if (*s2 == 0)
				s = s2;
			else
				if (*(++s2 - 1) == c)
					s = s2;
		}
	}
	return (0);
}

size_t	mas(char **ptr, const char *s, char c)
{
	char	*s1;
	char	*s2;
	char	*s3;

	s1 = (char *) s;
	if (c == 0)
	{
		s3 = s1;
		s1 = s3 + ft_strlen(s) + 1;
		return (mal(ptr, s3, s1, 0));
	}
	s2 = s1;
	return (mas2(ptr, s1, c, s2));
}

char	**ft_split(char const *s, char c)
{
	char	**ptr;
	size_t	i;
	size_t	j;

	i = 1;
	j = 0;
	ptr = 0;
	if (s != 0 && *s != 0)
		coufri(s, c, ptr, &i);
	ptr = (char **)ft_calloc(i, (sizeof(char *)));
	if (!ptr)
		return (0);
	while (j < i)
		ptr[j++] = 0;
	if (i != 1)
		j = mas(ptr, s, c);
	if (j == 5)
	{
		coufri(s, c, ptr, &j);
		return (0);
	}
	return (ptr);
}
