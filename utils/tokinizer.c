/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokinizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:46:34 by araji             #+#    #+#             */
/*   Updated: 2025/05/11 15:47:42 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*tokenize_input(t_general *ctx)
{
	t_token *(tokens), *(new);
	char *(token_value);
	t_token_type (token_type);
	int (len), (i);
	tokens = NULL;
	i = 0;
	while (ctx->input[i])
	{
		while (ctx->input[i] && is_whitespace(ctx->input[i]))
			i++;
		if (!ctx->input[i])
			break ;
		if (ctx->input[i] == '"' || ctx->input[i] == '\'')
		{
			len = handle_quotes(ctx, i, &token_value);
			if (len < 0)
				return (NULL);// cleanp()
			new = new_token(token_value, TOKEN_WORD, false);
			if (!new)
			{
				set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
				return (NULL);// cleanp()
			}
			add_token(&tokens, new);
			i += len;
		}
		else if (is_operator(ctx->input[i]))
		{
			len = handle_operator(ctx, i, &token_type, &token_value);
			if (len < 0)
				return (NULL);// cleanp()
			new = new_token(token_value, token_type, false);
			if (!new)
			{
				set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
				return (NULL);// cleanp()
			}
			add_token(&tokens, new);
			i += len;
		}
		else if (ctx->input[i] == '$')
		{
			len = handle_dollar(ctx, i, &token_value);
			if (len < 0)
				return (NULL);// cleanp()
			new = new_token(token_value, TOKEN_WORD, true);
			if (!new)
			{
				set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
				return (NULL);// cleanp()
			}
			add_token(&tokens, new);
			i += len;
		}
		else
		{
			len = handle_word(ctx, i, &token_value);
			if (len < 0)
				return (NULL);// cleanp()
			new = new_token(token_value, TOKEN_WORD, false);
			if (!new)
			{
				set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
				return (NULL);// cleanp()
			}
			add_token(&tokens, new);
			i += len;
		}
	}
	return (tokens);
}