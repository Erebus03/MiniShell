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

/* normal words with no whitespace or operators */
int	handle_word(t_general *ctx, int i, char **value)
{
	char *(result);
	int (start), (word_end), (result_size);
	start = i;
	word_end = i;
	while (ctx->input[word_end] && !is_whitespace(ctx->input[word_end])
		&& !is_operator(ctx->input[word_end]) && !is_quote(ctx->input[word_end])
		&& ctx->input[word_end] != '$')
		word_end++;
	result_size = calculate_expansion_size(ctx, start, ctx->input[word_end]);
	if (result_size < 0)
		return (-1);
	result = (char *)malloc(result_size + 1 * sizeof(char));
	if (!result)
	{
		set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
		return (-1);
	}
	if (build_exp_str(ctx, start, ctx->input[word_end], result) < 0)
	{
		free(result);
		return (-1);
	}
	*value = result;
	return (word_end - start);
}

/* Handles operator tokens (|, <, >, >>, <<) */
static void	set_operator_type(t_token_type *type, char c, char next, int *len)
{
	if (c == '|')
		*type = TPIPE;
	else if (c == '<')
	{
		if (next == '<')
		{
			*type = THEREDOC;
			*len = 2;
		}
		else
			*type = TREDIR_IN;
	}
	else if (c == '>')
	{
		if (next == '>')
		{
			*type = TREDIR_APPEND;
			*len = 2;
		}
		else
			*type = TREDIR_OUT;
	}
}

int	handle_operator(t_general *ctx, int i, t_token_type *type, char **value)
{
	int (len);
	char *(result);
	len = 1;
	set_operator_type(type, ctx->input[i], ctx->input[i + 1], &len);
	result = malloc(len + 1);
	if (!result)
		return (-1);
	ft_strncpy(result, ctx->input + i, len);
	result[len] = '\0';
	*value = result;
	return (len);
}

/*
int	handle_operator(t_general *ctx, int i, t_token_type *type, char **value)
{
	bool (has_next);
	char *(result);
	int (len);
	len = 1;
	has_next = (ctx->input[i + 1] != '\0');
	if (ctx->input[i] == '|')
	{
		*type = TPIPE;
	}
	else if (ctx->input[i] == '<')
	{
		if (has_next && ctx->input[i + 1] == '<')
		{
			*type = THEREDOC;
			len = 2;
		}
		else
			*type = TREDIR_IN;
	}
	else if (ctx->input[i] == '>')
	{
		if (has_next && ctx->input[i + 1] == '>')
		{
			*type = TREDIR_APPEND;
			len = 2;
		}
		else
			*type = TREDIR_OUT;
	}
	result = malloc(len + 1);
	if (!result)
		return (-1);
	ft_strncpy(result, ctx->input + i, len);
	result[len] = '\0';
	*value = result;
	return (len);
}
*/

/*	Handel quoted  strs
	real commands should splitted when passed to env vars (e.i. ls    -l -a) */
int	handle_quotes(t_general *ctx, int i, char **value)
{
	int (start), (result_size), (final_pos);
	char (quote_char);
	char *(result);
	quote_char = ctx->input[i];
	start = i;
	i++;
	result_size = calculate_expansion_size(ctx, i, quote_char);
	if (result_size < 0)
		return (-1);
	result = (char *)malloc(result_size + 1);
	if (!result)
		return (-1);
	final_pos = build_exp_str(ctx, i, quote_char, result);
	if (final_pos < 0)
	{
		free(result);
		return (-1);
	}
	*value = result;
	return (final_pos + 1 - start);
}

/* Handles var expansion ($VAR) */
int	handle_dollar(t_general *ctx, int i, char **value)
{
	char *(var_name);
	int (start), (var_len);
	var_len = 0;
	start = i++;
	if (ctx->input[i] == '?')
	{
		*value = ft_itoa(ctx->exit_status);
		return (2);
	}
	while (ctx->input[i] && (ft_isalnum(ctx->input[i]) || ctx->input[i] == '_'))
		increment_val(&i, &var_len, NULL, NULL);
	var_name = (char *)malloc(var_len + 1);
	if (!var_name)
		return (0);
	ft_memcpy(var_name, ctx->input + start + 1, var_len);
	var_name[var_len] = '\0';
	if (var_len > 0 && ctx->no_expand_heredoc == 0)
		*value = get_env_value(var_name, ctx->envlst);
	else if (var_len > 0 && ctx->no_expand_heredoc == 1)
		*value = ft_strjoin("$", var_name);
	else if (ctx->input[i] != '\0')
		*value = NULL;
	else
		*value = ft_strdup("$");
	return (i - start);
}
