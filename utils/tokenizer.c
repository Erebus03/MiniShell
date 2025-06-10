/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:46:34 by araji             #+#    #+#             */
/*   Updated: 2025/06/05 18:14:09 by araji            ###   ########.fr       */
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
		if (tokens && (last_token(tokens))->type == TOKEN_HEREDOC)
			ctx->no_expand_heredoc = 1;
		if ((ctx->input[i] == '"' || ctx->input[i] == '\''))
		{
			len = handle_quotes(ctx, i, &token_value);
			if (len < 0)
				return (NULL);	//	cleanup()
			if (token_value)   //	[0])
			{
				new = new_token(token_value, TOKEN_WORD, false);
				if (!new)
					return (NULL);	//	cleanp()
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
			if (token_value)
			{
				if (to_be_split(token_value))
				{
					new = split_token_value(token_value);
					if (is_whitespace(token_value[0]) || is_operator(token_value[0]))
						skipped = 1;
				}
				else
					new = new_token(token_value, TOKEN_WORD, true);
				if (!new)
					return (NULL);// cleanp()
				add_token(&tokens, new);
			}
			i += len;
		}
		else
		{
			len = handle_word(ctx, i, &token_value);
			if (len < 0)
				return (NULL);	// cleanp()
				
			/* 	WHY IM CHECKING THIS??	*/
			// if (to_be_split(token_value))
			// {
			// 	// printf("%s should be split\n", token_value);
			// 	new = split_token_value(token_value);
			// 	if (is_whitespace(token_value[0]) || is_operator(token_value[0]))
			// 		skipped = 1;
			// }
			// else
			new = new_token(token_value, TOKEN_WORD, false);
			
			if (!new)
				return (NULL);	// cleanp()
			add_token(&tokens, new);
			i += len;
		}
		
		printf("skipped = %d\n", skipped);
		if (tokens_size(tokens) > 1)
		{
			if (new->type == TOKEN_WORD && skipped == 0 && (new->prev)->type == TOKEN_WORD)
				join_tokens(tokens, new);
		}
		/*  RESET VARS	*/
		
		/*	I NEED TO KNOW WHY DID I SET THIS CONDITION   && ctx->input[i - len] != '$'	*/
		// if (token_value)
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
	
	printf("\njoining [%s] with [%s]\n\n", tokens->value, (tokens->next)->value);

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