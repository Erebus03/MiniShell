/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 19:22:32 by araji             #+#    #+#             */
/*   Updated: 2024/11/06 15:58:35 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*allocate_fill(const char *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*sub;

	i = 0;
	sub = (char *)malloc((len + 1) * sizeof(char));
	if (sub == NULL)
		return (NULL);
	while (s[start + i] && i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[len] = '\0';
	return (sub);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*sub;
	size_t	s_len;

	if (!s)
		return (NULL);
	i = 0;
	s_len = ft_strlen(s);
	if (((size_t)start >= s_len)
		|| (s[i] == '\0' && (len == 0 || len == 1)))
	{
		sub = (char *)malloc(1 * sizeof(char));
		if (!sub)
			return (NULL);
		sub[0] = '\0';
		return (sub);
	}
	if (len > s_len - start)
		len = s_len - start;
	return (allocate_fill(s, start, len));
}
