/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_stuff.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:58:27 by araji             #+#    #+#             */
/*   Updated: 2025/05/11 10:58:27 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Print all tokens in a command for debugging
 */
void	print_tokens(t_token *tokens)
{
	t_token	*current;
	char	*type_str[] = {"WORD", "PIPE", "REDIR_IN",
		"REDIR_OUT", "REDIR_APPEND", "HEREDOC"};

	current = tokens;
	while (current)
	{
		printf("Token: [%s] (%s)\n",
			current->value,
			type_str[current->type]);
		current = current->next;
	}
}

/**
 * Print a command's redirections for debugging
 */
void	print_redirections(t_redir *redirs)
{
	t_redir	*current;
	char	*type_str[] = {"WORD", "PIPE", "REDIR_IN",
		"REDIR_OUT", "REDIR_APPEND", "HEREDOC"};

	current = redirs;
	while (current)
	{
		printf("  Redirection: type=%s, file=%s\n",
			type_str[current->type],
			current->file);
		current = current->next;
	}
}

/**
 * Print all commands for debugging
 */
void print_commands(t_command *commands)
{
	// printf("commands(%p)[%s]\n", commands, commands->cmd[1]);
	int i = 1;
	t_command *current = commands;
	int j;
	while (current)
	{
		printf("Command %d:\n", i++);
		for (j = 0; current->cmd[j] != NULL; j++)
			printf(" [%s]", current->cmd[j]);
		printf("\n");
		if (current->redirs)
		{
			printf("  Redirections:\n");
			print_redirections(current->redirs);
		}
		printf("\nOriginal Tokens:\n");
		print_tokens(current->tokens);
		printf("\n");
		current = current->next;
	}
}
