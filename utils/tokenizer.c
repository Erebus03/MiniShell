/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:46:34 by araji             #+#    #+#             */
/*   Updated: 2025/05/19 20:05:19 by araji            ###   ########.fr       */
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
				return (NULL);	// cleanp()
			new = new_token(token_value, TOKEN_WORD, false);
			if (!new)
			{
				set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
				return (NULL);	// cleanp()
			}
			add_token(&tokens, new);
			i += len;
		}
		//join two words if no white space is between them
		if (tokens_size(tokens) > 1)
			if (new->type == TOKEN_WORD && skipped == 0)
			{
				printf("found words mlaqin\n");
				join_tokens(tokens, new, tokens_size(tokens));
			}
		skipped = 0;
	}
	return (tokens);
}

void	join_tokens(t_token *tokens, t_token *new, int size)
{
	printf("\njoining %s with last word\n", new->value);
	
	t_token *lastnode;
	char *str;
	int	i;
	
	str = NULL;
	i = 1;
	while (i++ < size - 1)
		tokens = tokens->next; 
	str = ft_strjoin(tokens->value, (tokens->next)->value);
	free(tokens->value);
	tokens->value = str;
	lastnode = tokens->next;
	tokens->next = NULL;
	free(lastnode->value);
	free(lastnode);
	// printf("node before last is %s\n", tokens->value);
}
