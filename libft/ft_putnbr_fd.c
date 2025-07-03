/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sogabrie <sogabrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 20:07:36 by sogabrie          #+#    #+#             */
/*   Updated: 2023/04/26 20:07:37 by sogabrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft(int nb, int fd)
{
	write (fd, "-", 1);
	nb = nb * -1;
	return (nb);
}

void	ft1(int nb, int fd)
{
	ft(nb, fd);
	ft_putnbr_fd(214748364, fd);
	write(fd, "8", 1);
}

int	ft2(int nb)
{
	int	a;

	a = 1;
	while (nb > 9)
	{
		a = a * 10;
		nb = nb / 10;
	}
	return (a);
}

void	ft_putnbr_fd(int n, int fd)
{
	int	a;
	int	b;

	if (n == -2147483648)
	{
		ft1 (-1, fd);
		return ;
	}
	if (n < 0)
		n = ft (n, fd);
	b = n;
	a = ft2 (n);
	while (a >= 10)
	{
		b = n / a + 48;
		n = n % a;
		a = a / 10;
		write (fd, &b, 1);
	}
	b = n + 48;
	write (fd, &b, 1);
}
