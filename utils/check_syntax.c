/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:15:40 by araji             #+#    #+#             */
/*   Updated: 2025/07/10 21:15:27 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	validate_quotes(char *input)
{
	int		i;
	char	quote_char;

	i = 0;
	while (input[i])
	{
		if (input[i] != '"' && input[i] != '\'')
		{
			i++;
			continue ;
		}
		quote_char = input[i++];
		while (input[i] && input[i] != quote_char)
			i++;
		if (!input[i])
			return (0);
		i++;
	}
	return (1);
}

static int	valid_heredoc_count(t_token *tkns)
{
	t_token	*tmp;
	int		count;

	tmp = tkns;
	count = 0;
	while (tmp)
	{
		if (tmp->type == THEREDOC)
			count++;
		tmp = tmp->next;
	}
	if (count > 16)
		return (0);
	return (1);
}

/* helper for norm */
static int	s_exitcode_nd_ret(t_general *ctx, int ret_value, int exit_stat)
{
	(void)ctx;
	generale.exit_status = exit_stat;
	return (ret_value);
}

int	check_syntax(t_general *ctx, t_token *all_tokens)
{
	t_token *(token);
	token = all_tokens;
	if (!valid_heredoc_count(all_tokens))
		return (s_exitcode_nd_ret(ctx, 0, 2));
	while (token)
	{
		if (token->type == TPIPE)
		{
			if (!token->prev || !token->next
				|| (token->next && token->next->type == TPIPE))
				return (s_exitcode_nd_ret(ctx, 0, 2));
		}
		if ((token->type != TWORD && token->type != TPIPE)
			&& (!token->next || (token->next && token->next->type != TWORD)))
			return (s_exitcode_nd_ret(ctx, 0, 2));
		token = token->next;
	}
	return (1);
}
