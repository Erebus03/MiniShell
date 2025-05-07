/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 21:59:15 by araji             #+#    #+#             */
/*   Updated: 2024/11/17 02:46:00 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t len)
{
	unsigned char	*p_dst;

	p_dst = (unsigned char *)dst;
	if (!dst && !src)
		return (NULL);
	while (len--)
	{
		*p_dst++ = *(unsigned char *)src++;
	}
	return (dst);
}
