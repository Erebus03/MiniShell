/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:33:19 by araji             #+#    #+#             */
/*   Updated: 2025/06/28 13:20:30 by araji            ###   ########.fr       */
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
int	process_dollar_token(t_general *ctx, int i, t_token **tokens, int *skipped,
		t_token **last_added)
{
	char *(token_value);
	t_token *(new);
	int (len);
	len = handle_dollar(ctx, i, &token_value);
	if (len < 0)
		return (-1);
	if (token_value)
	{
		if (to_be_split(token_value))
		{
			new = split_token_value(token_value);
			if (is_whitespace(token_value[0])
				|| is_operator(token_value[0]))
				*skipped = 1;
		}
		else
			new = new_token(token_value, TWORD, true);
		if (!new)
			return (-1);
		*last_added = new;
		add_token(tokens, new);
	}
	else
		*last_added = NULL;
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
	add_token(tokens, new);
	*last_added = new;
	return (len);
}
