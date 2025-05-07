/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:48:25 by araji             #+#    #+#             */
/*   Updated: 2024/11/17 02:06:39 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int ch, size_t len)
{
	unsigned char	*pstr;

	pstr = (unsigned char *)str;
	while (len--)
	{
		if (*pstr == (unsigned char)ch)
			return (pstr);
		pstr++;
	}
	return (NULL);
}
