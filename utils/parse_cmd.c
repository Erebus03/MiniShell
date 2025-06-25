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

void	check_ambigiuos_redir(void)
{
	return ;
}
/**
 * Split tokens into commands based on pipe operators
 */

t_command	*parse_command(t_general *ctx)
{
	t_token *(all_tokens), *(cmd_start), *(current), *(prev);
	t_command *(commands), *(new_cmd);
	all_tokens = tokenize_input(ctx);
	commands = NULL;
	cmd_start = all_tokens;
	current = all_tokens;
	prev = NULL;
	if (!all_tokens)
		return (NULL);
	print_tokens(all_tokens);
	if (!check_syntax(ctx, all_tokens))
	{
		printf("minishell syntax error : exited with %d\n", ctx->exit_status);
		return (NULL);
	}
	ctx->exit_status = 0;
	while (current)
	{
		if (current->type == TPIPE)
		{
			if (prev)
				prev->next = NULL;
			new_cmd = new_command();
			if (!new_cmd)
				return (NULL);
			new_cmd->tokens = cmd_start;
			if (process_tokens(new_cmd) != SUCCESS)
			{
				free_commands(&new_cmd);
				return (NULL);
			}
			add_command(&commands, new_cmd);
			free(current->value);
			cmd_start = current->next;
			free(current);
			current = cmd_start;
			prev = NULL;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
	if (cmd_start)
	{
		new_cmd = new_command();
		if (!new_cmd)
			return (NULL);
		new_cmd->tokens = cmd_start;
		if (process_tokens(new_cmd) != SUCCESS)
		{
			free_commands(&new_cmd);
			return (NULL);
		}
		add_command(&commands, new_cmd);
	}
	return (commands);
}
