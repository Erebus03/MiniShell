/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 19:22:12 by araji             #+#    #+#             */
/*   Updated: 2025/05/06 23:29:42 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*fullstr;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	fullstr = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!fullstr)
		return (NULL);
	ft_memcpy(fullstr, s1, s1_len);
	ft_memcpy(fullstr + s1_len, s2, s2_len);
	fullstr[s1_len + s2_len] = '\0';
	return (fullstr);
}
