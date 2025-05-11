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

int handle_quotes(t_general *ctx, int i, char **value)
{
    int start = i;
    char quote_char = ctx->input[i];
    int j = 0;
    char *result;
    
	printf("quote %c\n", ctx->input[i]);
    i++; // Skip opening quote
	printf("started at %c\n", ctx->input[i]);

    while (ctx->input[i] && ctx->input[i] != quote_char)
	{
        i++;
	}
    
	printf("skipped to %c\n", ctx->input[i]);
    if (!ctx->input[i]) // No closing quote found
    {
        set_error(ctx, ERROR_QUOTES, "Unclosed quote");
        return -1;
    }
    
    // Allocate memory for the result (without quotes)
    result = malloc(i - start);
    if (!result)
    {
        set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
        return -1;
    }
    
    // Copy string without quotes
    j = 0;
    start++; // Skip opening quote
    while (start < i)
        result[j++] = ctx->input[start++];
    result[j] = '\0';
    
    *value = result;
    return i + 2 - start + j; // Return total length processed
}

/**
 * Handles word tokens (non-whitespace, non-operator sequences)
 * Returns the number of characters processed
 */
int handle_word(t_general *ctx, int i, char **value)
{
    int start = i;
    char *result;
    int j = 0;
    
    while (ctx->input[i] && !is_whitespace(ctx->input[i]) && !is_operator(ctx->input[i]))
        i++;
    
    // Allocate memory for the result
    result = malloc(i - start + 1);
    if (!result)
    {
        set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
        return -1;
    }
    
    // Copy word
    j = 0;
    while (start < i)
        result[j++] = ctx->input[start++];
    result[j] = '\0';
    
    *value = result;
    return i - start + j; // Return total length processed
}

/**
 * Handles operator tokens (|, <, >, >>, <<)
 * Returns the number of characters processed
 */
int handle_operator(t_general *ctx, int i, t_token_type *type, char **value)
{
    int len = 1;
    char *result;
    
    // Safely check for double operators (with bound checking)
    bool has_next = (ctx->input[i + 1] != '\0');
    
    // Determine operator type and length
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
    
    // Allocate memory for the result
    result = malloc(len + 1);
    if (!result)
    {
        set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
        return -1;
    }
    
    // Copy operator
    strncpy(result, ctx->input + i, len);
    result[len] = '\0';
    
    *value = result;
    return len;
}


/**
 * Handles variable expansion ($VAR)
 * Returns the number of characters processed from input
 */
int handle_dollar(t_general *ctx, int i, char **value)
{
    int start = i;
    char var_name[256] = {0};
    int var_len = 0;
    
    i++; // Skip $
    
    // Special case for $?
    if (ctx->input[i] == '?')
    {
        char exit_str[12]; // More than enough for an integer
        snprintf(exit_str, sizeof(exit_str), "%d", ctx->exit_status);
        *value = strdup(exit_str);
        return 2; // $ + ?
    }
    
    // Extract variable name
    while (ctx->input[i] && (isalnum(ctx->input[i]) || ctx->input[i] == '_'))
    {
        if (var_len < 255)
            var_name[var_len++] = ctx->input[i];
        i++;
    }
    var_name[var_len] = '\0';
    
    // Get variable value
    if (var_len > 0)
        *value = get_env_value(var_name, ctx->envarr);
    else
        *value = strdup("$"); // Just $ without variable name
    
    return i - start;
}
