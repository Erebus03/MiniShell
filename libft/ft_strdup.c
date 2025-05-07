/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 16:57:21 by araji             #+#    #+#             */
/*   Updated: 2024/10/27 16:57:22 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*dest;
	char	*p_dst;

	dest = malloc(ft_strlen(src) + 1);
	if (!dest)
		return (NULL);
	p_dst = dest;
	while (*src)
		*p_dst++ = *src++;
	*p_dst = '\0';
	return (dest);
}
