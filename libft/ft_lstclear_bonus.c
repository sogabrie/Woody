/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sogabrie <sogabrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 20:06:38 by sogabrie          #+#    #+#             */
/*   Updated: 2023/04/26 20:06:39 by sogabrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*de;

	if (!del || !lst || !*lst)
		return ;
	while (*lst)
	{
		de = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = de;
	}
}
