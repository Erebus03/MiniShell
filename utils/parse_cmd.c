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

/*

	// for (t_token* tmp = all_tokens; tmp; tmp = tmp->next)
	// {
	// 	if (ft_strcmp("<", tmp->value) == 0
	// 		|| ft_strcmp(">", tmp->value) == 0
	// 		|| ft_strcmp(">>", tmp->value) == 0)
	// 	{				//  tmp->value[0] == '>' || tmp->value[0] == '<') {
	// 		if (tmp->next && tmp->next->type == TWORD) {
	// 			if (tmp->next->next && tmp->next->next->type == TWORD) {
	// 				printf("Bash :  ambiguous redirect\n");
	// 				exit(124);
	// 			}
	// 		}
	// 	}
	// }
*/

// void	check_ambigiuos_redir(void)
// {
// 	return ;
// }

/*
	* Split tokens into commands based on pipe operators
*/
static t_command	*create_and_process_command(t_token *cmd_start)
{
	t_command	*new_cmd;

	new_cmd = new_command();
	if (!new_cmd)
		return (NULL);
	new_cmd->tokens = cmd_start;
	if (process_tokens(new_cmd) != 1)
		return (NULL);
	return (new_cmd);
}

static int	handle_pipe_token(t_command **commands, t_token **cmd_start,
			t_token **current, t_token *prev)
{
	t_command	*new_cmd;

	if (prev)
		prev->next = NULL;
	new_cmd = create_and_process_command(*cmd_start);
	if (!new_cmd)
		return (0);
	add_command(commands, new_cmd);
	free((*current)->value);
	*cmd_start = (*current)->next;
	free(*current);
	*current = *cmd_start;
	return (1);
}

static t_command	*process_cmd_tokens(t_token *all_tokens,
										t_token **last_start)
{
	t_token *(cmd_start), *(current), *(prev);
	t_command *(commands);
	commands = NULL;
	cmd_start = all_tokens;
	current = all_tokens;
	prev = NULL;
	while (current)
	{
		if (current->type == TPIPE)
		{
			if (handle_pipe_token(&commands, &cmd_start, &current, prev) != 1)
				return (NULL);
			prev = NULL;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
	*last_start = cmd_start;
	return (commands);
}

static t_command	*finalize_last_command(t_command *cmds,
	t_token *cmd_start)
{
	t_command	*new_cmd;

	if (cmd_start)
	{
		new_cmd = create_and_process_command(cmd_start);
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
	print_tokens(all_tokens);
	if (!check_syntax(ctx, all_tokens))
	{
		printf("minishell syntax error : exited with %d\n", ctx->exit_status);
		return (NULL);
	}
	ctx->exit_status = 0;
	commands = process_cmd_tokens(all_tokens, &last_cmd_start);
	return (finalize_last_command(commands, last_cmd_start));
}
