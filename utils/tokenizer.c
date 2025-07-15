/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:46:34 by araji             #+#    #+#             */
/*   Updated: 2025/07/15 18:23:27 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Helper function to initialize tokenizer state
static void	init_tokenizer_state(int *indx, void **ptrs, t_general *ctx)
{
	ft_bzero(indx, sizeof(int) * 4);
	ft_bzero(ptrs, sizeof(ptrs));
	indx[1] = ft_strlen(ctx->input);
}

static void	check_export_ident(t_token *ident_tkn)
{
	if (ident_tkn->value)
	{
		if (ident_tkn->value[ft_strlen(ident_tkn->value) - 1] == '=')
		{
			ident_tkn->is_identif = 1;
		}
	}
}

// process and add a single token
static int	process_and_add_token(t_general *ctx, int *indx, void **tkn_ptrs)
{
	t_token	*lasttkn;
	int		token_len;

	token_len = process_single_token(ctx, indx[0], tkn_ptrs, &(indx[2]));
	if (token_len < 0)
		return (-1);
	lasttkn = last_token(tkn_ptrs[0]);
	if (lasttkn)
	{
		if (lasttkn->type == TWORD && lasttkn->prev && (lasttkn->prev)->is_export)
			check_export_ident(lasttkn);
		if (lasttkn->type == TWORD && lasttkn->prev && (lasttkn->prev)->is_identif)
			lasttkn->is_identif = 1;
	}
	if (tkn_ptrs[1])
		handle_token_joining(tkn_ptrs[0], (t_token **)&tkn_ptrs[1], indx[2]);
	return (token_len);
}

/* update position and state after token processing */
static void	update_position_and_state(t_general *ctx, int *indx,
		t_token *new)
{
	if ((indx[0] + indx[3]) <= indx[1])
		indx[0] += indx[3];
	else
	{
		indx[0] = indx[1];
		return ;
	}
	if (new && new->value && ((last_token(new))->no_join_after == 0))
		indx[2] = 0;
	if (is_whitespace(ctx->input[indx[0]]) || ctx->input[indx[0]] == '\0')
		ctx->no_expand_heredoc = 0;
}

/*
	i = indx[0]
	inputlen = indx[1]
	skipped = indx[2]
	len = indx[3]

	tkn_ptrs[0]	= tokens;
	tkn_ptrs[1]	= last token added;
		(more like first token of the batch added)
*/
t_token	*tokenize_input(t_general *ctx)
{
	int		indx[4];
	void	*tkn_ptrs[2];

	ft_memset(tkn_ptrs, 0, sizeof(tkn_ptrs));
	init_tokenizer_state(indx, tkn_ptrs, ctx);
	while (indx[0] < indx[1] && ctx->input[indx[0]])
	{
		skip_whitespace_and_track(ctx, &(indx[0]), &(indx[2]));
		if (!ctx->input[indx[0]])
			break ;
		if (tkn_ptrs[0] && (last_token(tkn_ptrs[0]))->type == THEREDOC)
			ctx->no_expand_heredoc = 1;
		indx[3] = process_and_add_token(ctx, indx, tkn_ptrs);
		if (indx[3] < 0)
			return (NULL);
		update_position_and_state(ctx, indx, tkn_ptrs[1]);
	}
	return (tkn_ptrs[0]);
}
