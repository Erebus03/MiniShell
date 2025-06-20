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

/**
 * Split tokens into commands based on pipe operators
 */
t_command	*parse_command(t_general *ctx)
{
	t_token *(all_tokens), *(cmd_start), *(current), *(prev);
	t_command *(commands), *(new_cmd);
	all_tokens = tokenize_input(ctx);

	/*	should i check for imbegious commands here or what
		
		cant check after splitting env_var cuz it also be split by operator
		or maybe check just the first two, there would necesserally be two tokens
		if theyre words => imbegious
		maybe that cant work either, what if there is one word in env_var,
		but there os already a word before


	*/
	/* for now */
	// for (t_token* tmp = all_tokens; tmp; tmp = tmp->next)
	// {
	// 	if (ft_strcmp("<", tmp->value) == 0
	// 		|| ft_strcmp(">", tmp->value) == 0
	// 		|| ft_strcmp(">>", tmp->value) == 0)
	// 	{				//  tmp->value[0] == '>' || tmp->value[0] == '<') {
	// 		if (tmp->next && tmp->next->type == TOKEN_WORD) {
	// 			if (tmp->next->next && tmp->next->next->type == TOKEN_WORD) {
	// 				printf("Bash :  ambiguous redirect\n");
	// 				exit(124);
	// 			}
	// 		}
	// 	}
	// }
	
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
		return NULL;
	}
	// printf("syntax good\n");
	ctx->exit_status = 0;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (prev)
				prev->next = NULL;
			new_cmd = new_command();
			if (!new_cmd)
				return (NULL);// cleanp()
			new_cmd->tokens = cmd_start;
			if (process_tokens(new_cmd) != SUCCESS)
			{
				set_error(ctx, ERROR_SYNTAX, "Syntax error in command");
				free_commands(&new_cmd);
				return (NULL);// cleanp()
			}
			add_command(&commands, new_cmd);
			free(current->value); // Free the pipe token value
			cmd_start = current->next;
			free(current); // Free the pipe token
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
		{
			set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
			return (NULL);// cleanup()
		}
		new_cmd->tokens = cmd_start;
		if (process_tokens(new_cmd) != SUCCESS)
		{
			set_error(ctx, ERROR_SYNTAX, "Syntax error in command");
			free_commands(&new_cmd);
			return (NULL);// cleanp()
		}
		add_command(&commands, new_cmd);
	}
	return (commands);
}

