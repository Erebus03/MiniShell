/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:58:10 by araji             #+#    #+#             */
/*   Updated: 2025/05/11 10:58:10 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_commands(t_command **commands)
{
	t_command *(current), *(next);
	int (i);
	if (!commands || !*commands)
		return ;
	current = *commands;
	while (current)
	{
		next = current->next;
		if (current->cmd)
		{
			i = -1;
			while (current->cmd[++i] != NULL)
				free(current->cmd[i]);
			free(current->cmd);
		}
		free_redirs(&current->redirs);
		free_tokens(&current->tokens);
		free(current);
		current = next;
	}
	*commands = NULL;
}

void	free_redirs(t_redir **redirs)
{
	t_redir	*current;
	t_redir	*next;

	if (!redirs || !*redirs)
		return ;
	current = *redirs;
	while (current)
	{
		next = current->next;
		free(current->file);
		free(current);
		current = next;
	}
	*redirs = NULL;
}

void	free_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens || !*tokens)
		return ;
	current = *tokens;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	*tokens = NULL;
}

void	free_envp(t_general *ctx, int mode)
{
	t_env_var *(tmp);
	int (i);
	if (ctx->envlst && (mode == 'l' || mode == 'b'))
	{
		while (ctx->envlst)
		{
			tmp = ctx->envlst;
			ctx->envlst = ctx->envlst->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			tmp = NULL;
		}
		free(ctx->envlst);
		ctx->envlst = NULL;
	}
	if (ctx->envarr && (mode == 'a' || mode == 'b'))
	{
		i = 0;
		while (ctx->envarr[i])
			free(ctx->envarr[i++]);
		free(ctx->envarr);
		ctx->envarr = NULL;
	}
}
