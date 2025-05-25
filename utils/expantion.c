/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expantion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 21:41:16 by araji             #+#    #+#             */
/*   Updated: 2025/05/22 21:48:56 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	calculate_expansion_size(t_general *ctx, int start, char stop_char)
{
	int		(i), (result_size), (processed);
	char	*(temp_value);
	result_size = 0;
	i = start;
	while (ctx->input[i] && ctx->input[i] != stop_char)
	{
		if (ctx->input[i] == '$')
		{
			processed = handle_dollar(ctx, i, &temp_value);
			if (processed <= 0)
				return (-1);
			if (temp_value)
			{
				result_size += ft_strlen(temp_value);
				free(temp_value);
			}
			i += processed;
		}
		else
		{
			result_size++;
			i++;
		}
	}
	return (result_size);
}

int	build_expanded_string(t_general *ctx, int start, char stop_char, char *result)
{
	int		i, j, processed;
	char	*temp_value;

	i = start;
	j = 0;
	while (ctx->input[i] && ctx->input[i] != stop_char)
	{
		if (ctx->input[i] == '$')
		{
			processed = handle_dollar(ctx, i, &temp_value);
			if (processed <= 0)
				return (-1);
			if (temp_value)
			{
				strcpy(result + j, temp_value);
				j += ft_strlen(temp_value);
				free(temp_value);
			}
			i += processed;
		}
		else
			result[j++] = ctx->input[i++];
	}
	result[j] = '\0';
	return (i);
}
