/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:15:40 by araji             #+#    #+#             */
/*   Updated: 2025/07/02 14:33:37 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	validate_quotes(t_general *ctx)
{
	int		i;
	char	quote_char;

	i = 0;
	while (ctx->input[i])
	{
		if (ctx->input[i] != '"' && ctx->input[i] != '\'')
		{
			i++;
			continue ;
		}
		quote_char = ctx->input[i++];
		while (ctx->input[i] && ctx->input[i] != quote_char)
			i++;
		if (!ctx->input[i])
			return (0);
		i++;
	}
	return (1);
}

/* helper for norm */
int	s_exitcode_nd_ret(t_general *ctx, int ret_value, int exit_stat)
{
	(void)ctx;
	generale.exit_status = exit_stat;
	return (ret_value);
}

int	check_syntax(t_general *ctx, t_token *all_tokens)
{
	t_token *(token);
	if (validate_quotes(ctx) == 0)
	{
		generale.exit_status = 2;
		return (0);
	}
	token = all_tokens;
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
		{
			return (s_exitcode_nd_ret(ctx, 0, 2));
		}
		token = token->next;
	}
	return (1);
}
