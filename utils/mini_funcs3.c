/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_funcs3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 02:26:02 by araji             #+#    #+#             */
/*   Updated: 2025/06/29 02:26:02 by araji            ###   ########.fr       */
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

void	join_tokens(t_token *tokens, t_token *new)
{
	t_token *(next_node);
	char *(str);
	str = NULL;
	while (tokens != new->prev)
		tokens = tokens->next;
	// printf("\njoining [%s]-[%s]\n\n", tokens->value, (tokens->next)->value);
	str = ft_strjoin(tokens->value, (tokens->next)->value);
	free(tokens->value);
	tokens->value = str;
	next_node = new->next;
	// free(new->value);
	//free(new);
	tokens->next = next_node;
	if (next_node)
		next_node->prev = tokens;
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
