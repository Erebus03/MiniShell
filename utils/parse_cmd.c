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
	commands = NULL;
	cmd_start = all_tokens;
	current = all_tokens;
	prev = NULL;
	if (!all_tokens)
		return (NULL);
	// printf("\n\n*****************************\n\n");
  	print_tokens(all_tokens);
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (prev)
				prev->next = NULL;
			new_cmd = new_command();
			if (!new_cmd)
			{
				set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
				return (NULL);// cleanp()
			}
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
			return (NULL);// cleanp()
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
	// printf("\n\n*****************************\n\n");
	// print_commands(commands);
	return (commands);
}

// void	parse_command(t_general *ctx)
// {
// 	char	*type_str[] = {"WORD", "PIPE", "REDIR_IN",
// 		"REDIR_OUT", "REDIR_APPEND", "HEREDOC"};

// 	t_token *(all_tokens), *(current);
// 	all_tokens = tokenize_input(ctx);
// 	current = all_tokens;
// 	while (current)
// 	{
// 		printf("Token: [%s](type: %s))\n",
// 			current->value,
// 			type_str[current->type]);
// 		current = current->next;
// 	}
// }
