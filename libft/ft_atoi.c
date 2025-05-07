/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 16:54:45 by araji             #+#    #+#             */
/*   Updated: 2025/05/06 23:28:41 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *s)
{
	int	sign;
	int	result;

	sign = 1;
	result = 0;
	while (*s == 32 || (*s >= 9 && *s <= 13))
		s++;
	if (*s == 43 || *s == 45)
	{
		if (*s == 45)
			sign *= -1;
		s++;
	}
	while (ft_isdigit(*s))
	{
		result = (result * 10) + (*s - 48);
		s++;
	}
	return (result * sign);
}
