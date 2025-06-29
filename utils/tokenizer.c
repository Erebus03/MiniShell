/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:46:34 by araji             #+#    #+#             */
/*   Updated: 2025/06/29 03:36:46 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Helper function to initialize tokenizer state
static void	init_tokenizer_state(int *indx, t_token **tokens, t_general *ctx)
{
	ft_bzero(indx, sizeof(int) * 4);
	*tokens = NULL;
	indx[1] = ft_strlen(ctx->input);
}

// heredoc expansion state
static void	update_heredoc_state(t_general *ctx, t_token *tokens)
{
	if (tokens && (last_token(tokens))->type == THEREDOC)
		ctx->no_expand_heredoc = 1;
}

// process and add a single token
static int	process_and_add_token(t_general *ctx, int *indx,
		t_token **tokens, t_token **last_added)
{
	int	token_len;

	token_len = process_single_token(ctx, indx[0], tokens, &(indx[2]), last_added); //5
	if (token_len < 0)
		return (-1);
	if (*last_added)
		handle_token_joining(*tokens, *last_added, indx[2]);
	return (token_len);
}

/* update position and state after token processing */
static void	update_position_and_state(t_general *ctx, int *indx,
		t_token *last_added)
{
	indx[0] += indx[3];
	if (last_added && last_added->value)
		indx[2] = 0;
	if (is_whitespace(ctx->input[indx[0]]) || ctx->input[indx[0]] == '\0')
		ctx->no_expand_heredoc = 0;
}

/*
   i = indx[0]
   inputlen = indx[1]
   skipped = indx[2]
   len = indx[3]
*/
t_token	*tokenize_input(t_general *ctx)
{
	int		indx[4];
	t_token	*tokens;
	t_token	*last_added;
	t_token *tkn_ptrs[2];
	init_tokenizer_state(indx, &tokens, ctx);
	while (indx[0] < indx[1] && ctx->input[indx[0]])
	{
		skip_whitespace_and_track(ctx, &(indx[0]), &(indx[2]));
		if (!ctx->input[indx[0]])
			break ;
		update_heredoc_state(ctx, tokens);
		indx[3] = process_and_add_token(ctx, indx, &tokens, &last_added);
		if (indx[3] < 0)
			return (NULL);
		update_position_and_state(ctx, indx, last_added);
	}
	return (tokens);
}
