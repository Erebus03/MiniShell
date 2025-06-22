/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:15:40 by araji             #+#    #+#             */
/*   Updated: 2025/06/22 17:16:32 by araji            ###   ########.fr       */
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

int	check_syntax(t_general *ctx, t_token *all_tokens)
{
	t_token *(token);
	if (validate_quotes(ctx) == 0)
	{
		ctx->exit_status = 2;
		return (0);
	}
	token = all_tokens;
	while (token)
	{
		if (token->type == TPIPE)
		{
			if (!token->prev || !token->next
				|| (token->next && token->next->type == TPIPE))
			{
				ctx->exit_status = 2;
				return (0);
			}
		}
		if ((token->type != TWORD
				&& token->type != TPIPE)
			&& (!token->next || (token->next && token->next->type != TWORD)))
		{
			ctx->exit_status = 2;
			return (0);
		}
		token = token->next;
	}
	return (1);
}
