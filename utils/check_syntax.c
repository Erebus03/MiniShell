/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:15:40 by araji             #+#    #+#             */
/*   Updated: 2025/06/20 17:53:03 by araji            ###   ########.fr       */
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

int check_syntax(t_general *ctx, t_token* all_tokens)
{
	if (validate_quotes(ctx) == 0)
	{
		ctx->exit_status = 2;
		return 0;
	}
	// printf("quotes are valid\n");
	t_token *token = all_tokens;
	while (token)
	{
		if (token->type == TOKEN_PIPE)
		{
			if (!token->prev || !token->next
				|| (token->next && token->next->type == TOKEN_PIPE))
			{
				ctx->exit_status = 2;
				return 0;
			}
		}
		if ((token->type != TOKEN_WORD
			&& token->type != TOKEN_PIPE)
			&& (!token->next || (token->next && token->next->type != TOKEN_WORD)))
		{
			ctx->exit_status = 2; // bash exits with 0
			return 0;
		}
		token = token->next;
	}
	// printf("synatx valid\n");
	/*
			Exit Code 2 - Confirmed Cases:
			 Unclosed quotes: echo "unclosed → Exit code 2✅
			✅ Pipe at start: | grep test → Exit code 2
			✅ Pipe at end: echo hello | → Exit code 2
			✅ Unclosed control structures: if [ test → Exit code 2
			✅ Unexpected tokens: done, echo (test) → Exit code 2
			Exit Code 127 - Command Not Found:
			✅ Typos: eccoh, ecoh, ech → Exit code 127
			Exit Code 0 - Surprising Results:
			❗ Redirection without command: > file → Exit code 0 (not 2!)
			This is actually correct bash behavior - > file creates an empty file and succeeds. It's syntactically valid (though unusual).
	*/	
	return 1;
	/* checl synatx */
}