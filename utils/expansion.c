/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expantion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 21:41:16 by araji             #+#    #+#             */
/*   Updated: 2025/05/26 00:37:41 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	calculate_expansion_size(t_general *ctx, int i, char stop_char) //	i = start index
{
	int		(result_size), (processed);
	char	*(temp_value);
	result_size = 0;
	while (ctx->input[i] && ctx->input[i] != stop_char)
	{
		if (ctx->input[i] == '$')
		{
			processed = handle_dollar(ctx, i, &temp_value);
			if (processed <= 0)	// error handling
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
			// split the input into tokens if the var has white spaces
			// link the tokens in a separate function
			// prossed the noraml work;
			if (processed <= 0)
				return (-1);
			if (temp_value)
			{
				strcpy(result + j, temp_value);	// build func
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

t_token	*split_token_value(t_token *token, char *value)
{
	t_token	*new_token;
	int		i, j, start;

	if (!value || !*value)
		return (token);
	i = 0;
	start = 0;
	while (value[i])
	{
		if (is_whitespace(value[i]) || is_operator(value[i]))
		{
			if (i > start)
			{
				new_token = new_token(strndup(value + start, i - start), TOKEN_WORD, false);
				add_token(&token, new_token);
			}
			if (is_operator(value[i]))
			{
				new_token = new_token(strndup(value + i, 1), TOKEN_OPERATOR, false);
				add_token(&token, new_token);
			}
			start = i + 1;
		}
		i++;
	}
	if (i > start)
	{
		new_token = new_token(strndup(value + start, i - start), TOKEN_WORD, false);
		add_token(&token, new_token);
	}
	return (token);
}
