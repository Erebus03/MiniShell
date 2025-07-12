/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_funcs3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 02:26:02 by araji             #+#    #+#             */
/*   Updated: 2025/07/12 18:28:35 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void join_tokens(t_token *tokens, t_token **new)
{
    t_token *(next_node);
    t_token *(to_be_freed);
    char *(str);
    while (tokens != (*new)->prev)
        tokens = tokens->next;
    // printf("\njoining [%s]-[%s]\n", tokens->value, (tokens->next)->value);
    str = ft_strjoin(tokens->value, (tokens->next)->value);
    free(tokens->value);
    tokens->value = str;
    to_be_freed = *new;
    next_node = (*new)->next;
    *new = next_node;
    free(to_be_freed->value);
    free(to_be_freed);
    tokens->next = next_node;
    if (next_node)
        next_node->prev = tokens;
}

// void	join_tokens(t_token *tokens, t_token **new)
// {
// 	t_token *(next_node);
// 	t_token *(to_be_freed);
// 	char *(str);
// 	str = NULL;
// 	while (tokens != (*new)->prev)
// 		tokens = tokens->next;
	
// 	printf("\njoining [%s]-[%s]\n", tokens->value, (tokens->next)->value);

// 	str = ft_strjoin(tokens->value, (tokens->next)->value);
// 	free(tokens->value);
// 	tokens->value = str;

// 	next_node = (*new)->next;
// 	to_be_freed = (*new);
// 	if ((*new))
// 		(*new) = (*new)->next;
// 	free(to_be_freed->value);
// 	free(to_be_freed);
	
// 	tokens->next = next_node;
// 	if (next_node)
// 		next_node->prev = tokens;
// }

/* Function to handle token joining logic */
void	handle_token_joining(t_token *tokens, t_token **new, int skipped)
{
	if (*new && (*new)->value && tokens_size(tokens) > 1)
	{
		if ((*new)->type == TWORD && skipped == 0 && (*new)->prev
			&& ((*new)->prev)->type == TWORD)
		{
			join_tokens(tokens, new);
		}
	}
}

/* used in tokenizer2.c to split values inside env variables if they should be
*/
t_token	*split_update_tknvalue(char *token_value, int *skipped)
{
	t_token	*new;

	new = split_token_value(token_value);
	if (is_whitespace(token_value[0]))
		*skipped = 1;
	if (is_whitespace(token_value[ft_strlen(token_value) - 1]))
		(last_token(new))->no_join_after = 1;
	return (new);
}
