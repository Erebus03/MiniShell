/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 11:53:25 by araji             #+#    #+#             */
/*   Updated: 2024/10/27 17:57:13 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int ch)
{
	char	*ps;

	ps = (char *)s;
	while (*ps)
	{
		if (*ps == (unsigned char)ch)
			return (ps);
		ps++;
	}
	if ((unsigned char)ch == '\0')
		return (ps);
	return (NULL);
}
