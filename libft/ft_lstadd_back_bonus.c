/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:20:24 by araji             #+#    #+#             */
/*   Updated: 2024/11/11 12:25:46 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list	*last_node;

	if (!alst || !new)
		return ;
	if (!*alst)
	{
		*alst = new;
		return ;
	}
	last_node = ft_lstlast(*alst);
	if (!last_node)
		*alst = new;
	last_node->next = new;
}
