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

/**
 * Process tokens to populate command argv and redirections
 */
t_error_code	process_tokens(t_command *cmd)
{
	t_token *(current), *(next);
	t_redir *(redir);
	int (arg_count);
	current = cmd->tokens;
	arg_count = 0;
	while (current)
	{
		if (current->type == TWORD)
			arg_count++;
		else if (current->type != TPIPE)
		{
			if (!current->next || current->next->type != TWORD)
				return (ERROR_SYNTAX); // Redir need a filename
			current = current->next; // Skip the file name
		}
		current = current->next;
	}
	cmd->cmd = (char **)malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->cmd)
		return (ERROR_MEMORY);
	current = cmd->tokens;
	arg_count = 0;
	while (current)
	{
		next = current->next;
		if (current->type == TWORD)
		{
			cmd->cmd[arg_count] = strdup(current->value);
			if (!cmd->cmd[arg_count])
				return (ERROR_MEMORY);
			arg_count++;
		}
		else if (current->type != TPIPE)
		{
			if (next && next->type == TWORD) // Create redirection
			{
				redir = new_redir(current->type, next->value);
				if (current->type == THEREDOC)
					redir->expand_in_heredec = (current->quoted_delim != 1);
				if (!redir)
					return (ERROR_MEMORY);
				add_redir(&cmd->redirs, redir);
				current = next; // Skip the file name
				next = current->next;
			}
		}
		current = next;
	}
	cmd->cmd[arg_count] = NULL;
	return (SUCCESS);
}
