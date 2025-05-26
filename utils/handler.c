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

int handle_word(t_general *ctx, int i, char **value)
{
	char	*(result);
	int		(start), (word_end), (result_size);
	start = i;
	word_end = i;
	while (ctx->input[word_end] && !is_whitespace(ctx->input[word_end])
		&& !is_operator(ctx->input[word_end]) && !is_quote(ctx->input[word_end]))
		word_end++;
	result_size = calculate_expansion_size(ctx, start, ctx->input[word_end]);
	if (result_size < 0)
		return (-1);
	result = (char *)ft_calloc(result_size + 1, sizeof(char));
	if (!result)
	{
		set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
		return (-1);
	}
	if (build_expanded_string(ctx, start, ctx->input[word_end], result) < 0)
	{
		free(result);
		return (-1);
	}
	*value = result;
	return (word_end - start);
}

/* Handles operator tokens (|, <, >, >>, <<) */

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

/*	Handel quoted  strs
	real commands should splitted when passed to env vars (e.i. ls    -l -a) */

int handle_quotes(t_general *ctx, int i, char **value)
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
	final_pos = build_expanded_string(ctx, i, quote_char, result);
	if (final_pos < 0)
	{
		free(result);
		return (-1);
	}
	*value = result;
	return (final_pos + 1 - start);
}

/* Handles var expansion ($VAR) */

// handel dollar should be fixed to handel more than 255 var names
int	handle_dollar(t_general *ctx, int i, char **value)
{
	char	var_name[256];
	// char	exit_str[12];

	int (start), (var_len);
	ft_memset(var_name, 0, sizeof(var_name));
	var_len = 0;
	start = i++; //skipped $ char
	if (ctx->input[i] == '?') // $?
	{
		printf("\nexit status >%d<\n", ctx->exit_status);
		/*	allocate for *value to put exit_status in
			fill it strdup(itoa(exit_status)))	*/
		return (2);
	}
	while (ctx->input[i] && (ft_isalnum(ctx->input[i]) || ctx->input[i] == '_'))
	{
		if (var_len < 255)
			var_name[var_len++] = ctx->input[i];
		i++;
	}
	var_name[var_len] = '\0';
	if (var_len > 0)
		*value = get_env_value(var_name, ctx->envlst);
	else
		*value = ft_strdup("$"); // Just $ without variable name
	return (i - start);
}


// int	handle_word_mine(t_general *ctx, int i, char **value)
// {
// 	char *(result);
// 	int (start), (j);
// 	j = 0;
// 	start = i;
// 	while (ctx->input[i] && !is_whitespace(ctx->input[i])
// 		&& !is_operator(ctx->input[i]) && !is_quote(ctx->input[i]))
// 	{
// 		/* IF A DOLLAR IS FOUND EXPAND THE VAR */
// 		i++;
// 	}
// 	result = malloc(i - start + 1);
// 	if (!result)
// 	{
// 		set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
// 		return (-1);
// 	}
// 	j = 0;
// 	while (start < i)
// 		result[j++] = ctx->input[start++];
// 	result[j] = '\0';
// 	*value = result;
// 	return (i - start + j);
// }


// int	handle_quotes_mine(t_general *ctx, int i, char **value)
// {
// 	char (quote_char);
// 	char *(result);
// 	int (j), (start);
// 	quote_char = ctx->input[i];
// 	start = i;
// 	i++;
// 	while (ctx->input[i] && ctx->input[i] != quote_char)
// 		i++;
// 	if (!ctx->input[i])
// 	{
// 		set_error(ctx, ERROR_QUOTES, "Unclosed quote");
// 		return (-1);
// 	}
// 	result = malloc(i - start);
// 	if (!result) //cleanup()
// 		return (-1);
// 	j = 0;
// 	start++;
// 	while (start < i)
// 		result[j++] = ctx->input[start++];
// 	result[j] = '\0';
// 	*value = result;
// 	return (i + 2 - start + j); // Return total length processed
// }
