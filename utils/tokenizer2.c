/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:33:19 by araji             #+#    #+#             */
/*   Updated: 2025/07/08 17:21:07 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Function to process quoted strings */
int	process_quoted_token(t_general *ctx, int i, t_token **tokens,
		t_token **last_added)
{
	char	*token_value;
	t_token	*new;
	int		len;

	len = handle_quotes(ctx, i, &token_value);
	if (len < 0)
		return (-1);
	if (token_value)
	{
		new = new_token(token_value, TWORD, false);
		if (!new)
			return (-1);
		if (ctx->no_expand_heredoc == 1)
			new->quoted_delim = 1;
		add_token(tokens, new);
		*last_added = new;
	}
	else
		*last_added = NULL;
	return (len);
}

/* Function to process operators */
int	process_operator_token(t_general *ctx, int i, t_token **tokens,
		t_token **last_added)
{
	char			*token_value;
	t_token_type	token_type;
	t_token			*new;
	int				len;

	len = handle_operator(ctx, i, &token_type, &token_value);
	if (len < 0)
		return (-1);
	new = new_token(token_value, token_type, false);
	if (!new)
		return (-1);
	add_token(tokens, new);
	*last_added = new;
	return (len);
}

/* Function to process dollar expansions */
int	process_dollar_token(t_general *ctx, int i, void **tkn_ptrs, int *skipped)
{
	char *(token_value);
	t_token *(new);
	t_token *(last_tkn);
	int (len);
	len = handle_dollar(ctx, i, &token_value);
	if (len < 0)
		return (-1);
	if (token_value)
	{
		last_tkn = last_token(tkn_ptrs[0]);
		if (to_be_split(token_value) && (last_tkn && last_tkn->is_identif == 0
			&& !is_token_op(last_tkn)))
		{
			new = split_update_tknvalue(token_value, skipped);
		}
		else
			new = new_token(token_value, TWORD, true);
		if (!new)
			return (-1);
		tkn_ptrs[1] = new;
		add_token((t_token **)&tkn_ptrs[0], new);
	}
	else
		tkn_ptrs[1] = NULL;
	return (len);
}

/* Function to process regular words */
int	process_word_token(t_general *ctx, int i, t_token **tokens,
		t_token **last_added)
{
	char	*token_value;
	t_token	*new;
	int		len;

	len = handle_word(ctx, i, &token_value);
	if (len < 0)
		return (-1);
	new = new_token(token_value, TWORD, false);
	if (!new)
		return (-1);
	if (ft_strcmp(new->value, "export") == 0)
		new->is_export = 1;
	add_token(tokens, new);
	*last_added = new;
	return (len);
}

/* Helper to process a single token based on current character */
int	process_single_token(t_general *ctx, int i, void **tkn_ptrs, int *skipped)
{
	if (ctx->input[i] == '"' || (ctx->input[i] == '\''
			&& ctx->inside_env_var == 0))
		return (process_quoted_token(ctx, i, (t_token **)&tkn_ptrs[0],
				(t_token **)&tkn_ptrs[1]));
	else if (is_operator(ctx->input[i]) && ctx->inside_env_var == 0)
		return (process_operator_token(ctx, i, (t_token **)&tkn_ptrs[0],
				(t_token **)&tkn_ptrs[1]));
	else if (ctx->input[i] == '$')
		return (process_dollar_token(ctx, i, tkn_ptrs, skipped));
	else
		return (process_word_token(ctx, i, (t_token **)&tkn_ptrs[0],
				(t_token **)&tkn_ptrs[1]));
}
