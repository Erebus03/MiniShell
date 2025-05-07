/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:59:55 by araji             #+#    #+#             */
/*   Updated: 2024/10/27 17:44:03 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t	size)
{
	size_t	i;
	size_t	dlen;
	size_t	slen;

	slen = ft_strlen(src);
	if (size == 0 || size <= ft_strlen(dest))
		return (slen + size);
	dlen = ft_strlen(dest);
	i = 0;
	while (src[i] && ((dlen + i) < size - 1))
	{
		dest[dlen + i] = src[i];
		i++;
	}
	dest[dlen + i] = '\0';
	return (dlen + slen);
}
