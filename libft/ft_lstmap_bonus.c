/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:22:04 by araji             #+#    #+#             */
/*   Updated: 2024/11/11 12:32:46 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_node;
	t_list	*edited_node;
	void	*edited_content;

	if (!lst || !f || !del)
		return (NULL);
	new_node = NULL;
	while (lst)
	{
		edited_content = f(lst->content);
		edited_node = ft_lstnew(edited_content);
		if (!edited_node)
		{
			del(edited_content);
			ft_lstclear(&new_node, del);
			return (NULL);
		}
		ft_lstadd_back(&new_node, edited_node);
		lst = lst->next;
	}
	return (new_node);
}
