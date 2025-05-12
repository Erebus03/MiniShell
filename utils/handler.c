/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:58:12 by araji             #+#    #+#             */
/*   Updated: 2025/05/11 10:58:12 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_quotes(t_general *ctx, int i, char **value)
{
	char (quote_char);
	char *(result);
	int (j), (start);
	quote_char = ctx->input[i];
	start = i;
	i++;
	while (ctx->input[i] && ctx->input[i] != quote_char)
		i++;
	if (!ctx->input[i])
	{
		set_error(ctx, ERROR_QUOTES, "Unclosed quote");
		return (-1);
	}
	result = malloc(i - start);
	if (!result) //cleanup()
		return (-1);
	j = 0;
	start++;
	while (start < i)
		result[j++] = ctx->input[start++];
	result[j] = '\0';
	*value = result;
	return (i + 2 - start + j); // Return total length processed
}

/**
 * Handles word tokens (non-whitespace, non-operator sequences)
 * Returns the number of characters processed
 */
int	handle_word(t_general *ctx, int i, char **value)
{
	char *(result);
	int (start), (j);
	j = 0;
	start = i;
	while (ctx->input[i] && !is_whitespace(ctx->input[i])
		&& !is_operator(ctx->input[i]))
		i++;
	result = malloc(i - start + 1);
	if (!result)
	{
		set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
		return (-1);
	}
	j = 0;
	while (start < i)
		result[j++] = ctx->input[start++];
	result[j] = '\0';
	*value = result;
	return (i - start + j);
}

/**
 * Handles operator tokens (|, <, >, >>, <<)
 * Returns the number of characters processed
 */
int	handle_operator(t_general *ctx, int i, t_token_type *type, char **value)
{
	bool (has_next);
	char *(result);
	int (len);
	len = 1;
	has_next = (ctx->input[i + 1] != '\0');
	if (ctx->input[i] == '|')
	{
		*type = TOKEN_PIPE;
	}
	else if (ctx->input[i] == '<')
	{
		if (has_next && ctx->input[i + 1] == '<')
		{
			*type = TOKEN_HEREDOC;
			len = 2;
		}
		else
			*type = TOKEN_REDIR_IN;
	}
	else if (ctx->input[i] == '>')
	{
		if (has_next && ctx->input[i + 1] == '>')
		{
			*type = TOKEN_REDIR_APPEND;
			len = 2;
		}
		else
			*type = TOKEN_REDIR_OUT;
	}
	result = malloc(len + 1);
	if (!result)
	{
		set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
		return (-1);
	}
	strncpy(result, ctx->input + i, len);// make it
	result[len] = '\0';
	*value = result;
	return (len);
}

/**
 * Handles variable expansion ($VAR)
 * Returns the number of characters processed from input
 */
int	handle_dollar(t_general *ctx, int i, char **value)
{
	char	var_name[256];
	char	exit_str[12];

	int (start), (var_len);
	ft_memset(var_name, 0, sizeof(var_name));
	var_len = 0;
	start = i++; //skipped $ char
	if (ctx->input[++i] == '?') // $?
	{
		snprintf(exit_str, sizeof(exit_str), "%d", ctx->exit_status);
		*value = strdup(exit_str);
		return (2);
	}
	while (ctx->input[i] && (isalnum(ctx->input[i]) || ctx->input[i] == '_'))
	{
		if (var_len < 255)
			var_name[var_len++] = ctx->input[i];
		i++;
	}
	var_name[var_len] = '\0';
	if (var_len > 0)
		*value = get_env_value(var_name, ctx->envarr);
	else
		*value = strdup("$"); // Just $ without variable name
	return (i - start);
}
