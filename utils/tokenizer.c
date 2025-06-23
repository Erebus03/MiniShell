/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:46:34 by araji             #+#    #+#             */
/*   Updated: 2025/06/23 22:15:05 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*tokenize_input(t_general *ctx)
{
	t_token *(tokens), *(new);
	char *(token_value);
	t_token_type (token_type);
	int (len), (i), (skipped), (inputlen);
	tokens = NULL;
	i = 0;
	inputlen = ft_strlen(ctx->input);
	skipped = 0;
	while (i < inputlen && ctx->input[i])
	{
		while (ctx->input[i] && is_whitespace(ctx->input[i]))
		{
			i++;
			skipped = 1;
		}
		if (!ctx->input[i])
			break ;
		if (tokens && (last_token(tokens))->type == THEREDOC)
			ctx->no_expand_heredoc = 1;
		if ((ctx->input[i] == '"' || ctx->input[i] == '\''))
		{
			len = handle_quotes(ctx, i, &token_value);
			if (len < 0)
				return (NULL);
			if (token_value)
			{
				new = new_token(token_value, TWORD, false);
				if (!new)
					return (NULL);
				if (ctx->no_expand_heredoc == 1)
					new->quoted_delimliter = 1;
				add_token(&tokens, new);
			}
			i += len;
		}
		else if (is_operator(ctx->input[i]))
		{
			len = handle_operator(ctx, i, &token_type, &token_value);
			if (len < 0)
				return (NULL);
			new = new_token(token_value, token_type, false);
			if (!new)
				return (NULL);
			add_token(&tokens, new);
			i += len;
		}
		else if (ctx->input[i] == '$')
		{
			len = handle_dollar(ctx, i, &token_value);
			if (len < 0)
				return (NULL);
			if (token_value)
			{
				if (to_be_split(token_value))
				{
					new = split_token_value(token_value);
					if (is_whitespace(token_value[0])
						|| is_operator(token_value[0]))
						skipped = 1;
				}
				else
					new = new_token(token_value, TWORD, true);
				if (!new)
					return (NULL);
				add_token(&tokens, new);
			}
			i += len;
		}
		else
		{
			len = handle_word(ctx, i, &token_value);
			if (len < 0)
				return (NULL);
			new = new_token(token_value, TWORD, false);
			if (!new)
				return (NULL);
			add_token(&tokens, new);
			i += len;
		}
		if (token_value && tokens_size(tokens) > 1)
		{
			if (new->type == TWORD && skipped == 0 && new->prev
				&& (new->prev)->type == TWORD)
				join_tokens(tokens, new);
		}
		if (token_value)
			skipped = 0;
		if (is_whitespace(ctx->input[i + 1]) || ctx->input[i + 1] == '\0')
			ctx->no_expand_heredoc = 0;
	}
	return (tokens);
}

void	join_tokens(t_token *tokens, t_token *new)
{
	t_token *(next_node);
	char *(str);
	str = NULL;
	while (tokens != new->prev)
		tokens = tokens->next;
	printf("\njoining [%s]-[%s]\n\n", tokens->value, (tokens->next)->value);
	str = ft_strjoin(tokens->value, (tokens->next)->value);
	free(tokens->value);
	tokens->value = str;
	next_node = new->next;
	free(new->value);
	free(new);
	tokens->next = next_node;
	if (next_node)
		next_node->prev = tokens;
}
