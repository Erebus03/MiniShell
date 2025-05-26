/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:58:17 by araji             #+#    #+#             */
/*   Updated: 2025/05/11 10:58:17 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_error(t_general *ctx, t_error_code code, char *msg)
{
	ctx->error = code;
	if (ctx->error_msg)
		free(ctx->error_msg);
	ctx->error_msg = strdup(msg);
}

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

bool	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

bool	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

int	tokens_size(t_token *lst)
{
	int	i;

	i = 0;
	while (lst != NULL)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

int	to_be_split(char *value)
{
	int	(i);
	i = 0;
	while (value[i])
	{
		if (is_whitespace(value[i]) || is_operator(value[i]))
			return (1);
		i++;
	}
	return (0);
}