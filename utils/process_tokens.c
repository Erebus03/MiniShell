/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:58:29 by araji             #+#    #+#             */
/*   Updated: 2025/05/11 10:58:29 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_args(t_token *tokens)
{
	t_token	*current;
	int		arg_count;

	current = tokens;
	arg_count = 0;
	while (current)
	{
		if (current->type == TWORD)
			arg_count++;
		else if (current->type != TPIPE)
			current = current->next;
		current = current->next;
	}
	return (arg_count);
}

static int	process_token_word(t_general *ctx, t_command *cmd,
		t_token *current, int *arg_count)
{
	cmd->cmd[*arg_count] = ft_strdup(current->value);
	add_addr(ctx, new_addr((void *)(cmd->cmd[*arg_count])));
	if (!cmd->cmd[*arg_count])
		return (0);
	(*arg_count)++;
	return (1);
}

static int	process_redir_token(t_general *ctx, t_command *cmd,
		t_token *current, t_token *next)
{
	t_redir	*redir;

	redir = new_redir(ctx, current->type, next->value);
	if (current->type == THEREDOC)
		redir->expand_in_heredec = (current->quoted_delim != 1);
	if (!redir)
		return (0);
	add_redir(&cmd->redirs, redir);
	return (1);
}

static int	fill_cmd_array(t_general *ctx, t_command *cmd)
{
	t_token *(current),*(next);
	int (arg_count), (result);
	current = cmd->tokens;
	arg_count = 0;
	while (current)
	{
		next = current->next;
		if (current->type == TWORD)
		{
			result = process_token_word(ctx, cmd, current, &arg_count);
			if (result != 1)
				return (result);
		}
		else if (current->type != TPIPE)
		{
			result = process_redir_token(ctx, cmd, current, next);
			if (result != 1)
				return (result);
			current = next;
			next = current->next;
		}
		current = next;
	}
	cmd->cmd[arg_count] = NULL;
	return (1);
}

int	process_tokens(t_general *ctx, t_command *cmd)
{
	int	arg_count;

	arg_count = count_args(cmd->tokens);
	cmd->cmd = (char **)allocate(ctx, sizeof(char *) * (arg_count + 1));
	if (!cmd->cmd)
		return (0);
	return (fill_cmd_array(ctx, cmd));
}
