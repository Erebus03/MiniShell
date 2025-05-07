/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 16:55:24 by araji             #+#    #+#             */
/*   Updated: 2024/10/27 20:59:01 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*array;

	if (size && (count > (SIZE_MAX / size)))
		return (malloc(0));
	array = malloc(count * size);
	if (!array)
		return (NULL);
	ft_bzero(array, (count * size));
	return (array);
}
