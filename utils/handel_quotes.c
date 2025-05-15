/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:33:29 by araji             #+#    #+#             */
/*   Updated: 2025/05/14 16:58:34 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int calculate_quoted_size(t_general *ctx, int i, char quote_char)
{
	int (j), (result_size), (processed);
	char *(temp_value);
	result_size = 0;
	j = i;
	while (ctx->input[j] && ctx->input[j] != quote_char)
	{
		if (quote_char == '"' && ctx->input[j] == '$')
		{
			processed = handle_dollar(ctx, j, &temp_value);
			if (processed <= 0)
				return (-1);
			if (temp_value)
			{
				result_size += ft_strlen(temp_value);
				free(temp_value);
			}
			j += processed;
		}
		else
		{
			result_size++;
			j++;
		}
	}
	if (!ctx->input[j])
	{
		set_error(ctx, ERROR_QUOTES, "Unclosed quote");
		return (-1);
	}
	return (result_size);
}

int build_quoted_string(t_general *ctx, int i, char quote_char, char *result)
{
	char *(temp_value);
	int (j), (processed);
	j = 0;
	while (ctx->input[i] && ctx->input[i] != quote_char)
	{
		if (quote_char == '"' && ctx->input[i] == '$')
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

int handle_quotes(t_general *ctx, int i, char **value)
{
	int (start), (result_size), (final_pos);
	char (quote_char);
	char *(result);
	quote_char = ctx->input[i];
	start = i;
	i++;
	result_size = calculate_quoted_size(ctx, i, quote_char);
	if (result_size < 0)
		return (-1);
	result = (char *)malloc(result_size + 1);
	if (!result)
		return (-1);
	final_pos = build_quoted_string(ctx, i, quote_char, result);
	if (final_pos < 0)
	{
		free(result);
		return (-1);
	}
	*value = result;
	return (final_pos + 1 - start);
}
