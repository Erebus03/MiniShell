/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:46:34 by araji             #+#    #+#             */
/*   Updated: 2025/06/28 14:22:44 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*  REDIRECTION ARE NOT REDIRS IF INSIDE A ENV VARIABLE, FIX THIS	*/
/* Helper function to skip whitespace and track if any was skipped */
int	skip_whitespace_and_track(t_general *ctx, int *i, int *skipped)
{
	int	start_pos;

	start_pos = *i;
	while (ctx->input[*i] && is_whitespace(ctx->input[*i]))
		(*i)++;
	if (*i > start_pos)
		*skipped = 1;
	return (*i > start_pos);
}

/* Function to handle token joining logic */
void	handle_token_joining(t_token *tokens, t_token *new, int skipped)
{
	if (new && new->value && tokens_size(tokens) > 1)
	{
		if (new->type == TWORD && skipped == 0 && new->prev
			&& (new->prev)->type == TWORD)
		{
			join_tokens(tokens, new);
		}
	}
}

/* Helper to process a single token based on current character */
int	process_single_token(t_general *ctx, int i, t_token **tokens,
		int *skipped, t_token **last_added)
{
	if (ctx->input[i] == '"' || ctx->input[i] == '\'')
		return (process_quoted_token(ctx, i, tokens, last_added));
	else if (is_operator(ctx->input[i]) && ctx->inside_env_var == 0)
		return (process_operator_token(ctx, i, tokens, last_added));
	else if (ctx->input[i] == '$')
		return (process_dollar_token(ctx, i, tokens, skipped, last_added));
	else
		return (process_word_token(ctx, i, tokens, last_added));
}

/* Main tokenization function - refactored */
/* int (i), (inputlen), (skipped), (len);
   i = indx[0]
   inputlen = indx[1]
   skipped = indx[2]
   len = indx[3]
*/
t_token	*tokenize_input(t_general *ctx)
{
	int	indx[4];
	t_token *(tokens), *(last_added);
	tokens = NULL;
	// ft_bzero(&indx, sizeof(indx));
	indx[0] = 0;
	indx[2] = 0;
	indx[1] = ft_strlen(ctx->input);
	while (indx[0] < indx[1] && ctx->input[indx[0]])
	{
		skip_whitespace_and_track(ctx, &(indx[0]), &(indx[2]));
		if (!ctx->input[indx[0]])
			break ;
		if (tokens && (last_token(tokens))->type == THEREDOC)
			ctx->no_expand_heredoc = 1;
		indx[3] = process_single_token(ctx, indx[0], &tokens, &(indx[2]), &last_added);
		if (indx[3] < 0)
			return (NULL);
		if (last_added)
			handle_token_joining(tokens, last_added, indx[2]);
		indx[0] += indx[3];
		if (last_added && last_added->value)
			indx[2] = 0;
		if (is_whitespace(ctx->input[indx[0]]) || ctx->input[indx[0]] == '\0')
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
