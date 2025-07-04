/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:58:24 by araji             #+#    #+#             */
/*   Updated: 2025/05/11 10:58:24 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	check_ambigiuos_redir(void)
// {
// 	return ;
// }

/*
	* Split tokens into commands based on pipe operators
*/
static t_command	*create_and_process_command(t_general *ctx,
		t_token *cmd_start)
{
	t_command	*new_cmd;

	new_cmd = new_command(ctx);
	if (!new_cmd)
		return (NULL);
	new_cmd->tokens = cmd_start;
	if (process_tokens(new_cmd) != 1)
		return (NULL);
	return (new_cmd);
}

static int	handle_pipe_token(t_general *ctx, t_command **commands, void **ptrs)
{
	t_command	*new_cmd;

	if (ptrs[2])
		((t_token *)ptrs[2])->next = NULL;
	new_cmd = create_and_process_command(ctx, ptrs[0]);
	if (!new_cmd)
		return (0);
	add_command(commands, new_cmd);
	free(((t_token *)ptrs[1])->value);
	ptrs[0] = ((t_token *)ptrs[1])->next;
	free(ptrs[1]);
	ptrs[1] = ptrs[0];
	return (1);
}

/*
	t_token *(cmd_start), *(current), *(prev);
	ptrs[0] = cmd_start
	ptrs[1] = current
	ptrs[2] = prev
*/
static t_command	*process_cmd_tokens(t_general *ctx,
		t_token *all_tokens, t_token **last_start)
{
	void		*ptrs[3];
	t_command	*commands;

	commands = NULL;
	ptrs[0] = all_tokens;
	ptrs[1] = all_tokens;
	ptrs[2] = NULL;
	while (ptrs[1])
	{
		if (((t_token *)ptrs[1])->type == TPIPE)
		{
			if (handle_pipe_token(ctx, &commands, ptrs) != 1)
				return (NULL);
			ptrs[2] = NULL;
		}
		else
		{
			ptrs[2] = ptrs[1];
			ptrs[1] = ((t_token *)ptrs[1])->next;
		}
	}
	*last_start = ptrs[0];
	return (commands);
}

static t_command	*finalize_last_command(t_general *ctx, t_command *cmds,
	t_token *cmd_start)
{
	t_command	*new_cmd;

	if (cmd_start)
	{
		new_cmd = create_and_process_command(ctx, cmd_start);
		if (!new_cmd)
			return (NULL);
		add_command(&cmds, new_cmd);
	}
	return (cmds);
}

t_command	*parse_command(t_general *ctx)
{
	t_token		*all_tokens;
	t_token		*last_cmd_start;
	t_command	*commands;

	all_tokens = tokenize_input(ctx);
	if (!all_tokens)
		return (NULL);
	printf("\n\n");
	print_tokens(all_tokens);
	printf("\n\n");
	if (!check_syntax(ctx, all_tokens))
	{
		printf("minishell syntax error : exited with %d\n", generale.exit_status);
		return (NULL);
	}
	generale.exit_status = 0;
	commands = process_cmd_tokens(ctx, all_tokens, &last_cmd_start);
	return (finalize_last_command(ctx, commands, last_cmd_start));
}
