/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:24:46 by araji             #+#    #+#             */
/*   Updated: 2024/10/27 18:04:52 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int ch)
{
	size_t	len;
	char	*ps;

	len = ft_strlen(s);
	ps = (char *)s + len;
	while (len > 0)
	{
		if (*ps == (unsigned char)ch)
			return (ps);
		ps--;
		len--;
	}
	if (*ps == (unsigned char)ch)
		return (ps);
	return (NULL);
}
