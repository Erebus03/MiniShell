/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:46:34 by araji             #+#    #+#             */
/*   Updated: 2025/05/27 03:22:53 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*tokenize_input(t_general *ctx)
{
	t_token *(tokens), *(new);
	char *(token_value);
	t_token_type (token_type);
	int (len), (i), (skipped);
	tokens = NULL;
	i = 0;
	skipped = 0;
	while (ctx->input[i])
	{
		while (ctx->input[i] && is_whitespace(ctx->input[i]))
		{
			i++;
			skipped = 1;
		}
		if (!ctx->input[i])
			break ;
		if ((ctx->input[i] == '"' || ctx->input[i] == '\''))
		{
			len = handle_quotes(ctx, i, &token_value);
			if (len < 0)
				return (NULL);	//	cleanup()
			
			new = new_token(token_value, TOKEN_WORD, false);
			if (!new)
				return (NULL);	//	cleanp()
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
				return (NULL);// cleanp()
			add_token(&tokens, new);
			i += len;
		}
		else if (ctx->input[i] == '$')
		{
			len = handle_dollar(ctx, i, &token_value);
			if (len < 0)
				return (NULL);// cleanp()
			if (to_be_split(token_value))
			{
				new = split_token_value(token_value);
				skipped = 1;
			}
			else
				new = new_token(token_value, TOKEN_WORD, true);
			if (!new)
				return (NULL);// cleanp()
			add_token(&tokens, new);
			i += len;
		}
		else
		{
			len = handle_word(ctx, i, &token_value);
			if (len < 0)
				return (NULL);	// cleanp()
			if (to_be_split(token_value))
			{
				printf("%s should be split\n", token_value);
				new = split_token_value(token_value);
				skipped = 1;
			}
			else
				new = new_token(token_value, TOKEN_WORD, false);
			if (!new)
				return (NULL);	// cleanp()
			add_token(&tokens, new);
			i += len;
		}

		if (tokens_size(tokens) > 1)
			if (new->type == TOKEN_WORD && skipped == 0 && (last_token(tokens)->prev)->type == TOKEN_WORD)
				join_tokens(tokens, tokens_size(tokens));
		skipped = 0;
	}
	return (tokens);
}

void	join_tokens(t_token *tokens, int size)
{
	t_token *(lastnode);
	char *(str);
	int	(i);
	str = NULL;
	i = 1;
	while (i++ < size - 1)
		tokens = tokens->next;
	
	printf("\njoining [%s] with [%s]\n\n", tokens->value, (tokens->next)->value);
	str = ft_strjoin(tokens->value, (tokens->next)->value);
	free(tokens->value);
	tokens->value = str;
	lastnode = tokens->next;
	tokens->next = NULL;
	free(lastnode->value);
	free(lastnode);
}

