/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 23:29:32 by araji             #+#    #+#             */
/*   Updated: 2024/11/17 01:52:44 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	count_digits(int n)
{
	int	digit;

	digit = 0;
	if (n <= 0)
		digit++;
	while (n)
	{
		n = n / 10;
		digit++;
	}
	return (digit);
}

void	fill(char *result, int n, int pos)
{
	while (n > 0)
	{
		result[pos--] = ((n % 10) + 48);
		n /= 10;
	}
}

char	*ft_itoa(int n)
{
	char	*result;
	int		len;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	len = count_digits(n);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	if (n < 0)
	{
		result[0] = '-';
		n *= -1;
	}
	fill (result, n, len - 1);
	result[len] = '\0';
	return (result);
}
