/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_nodes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:58:20 by araji             #+#    #+#             */
/*   Updated: 2025/05/11 10:58:20 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Creates a new token
 */
t_token	*new_token(char *value, t_token_type type, bool t_general)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->t_general = t_general;
	token->next = NULL;
	return (token);
}

/**
 * Creates a new command
 */
t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->cmd = NULL;
	cmd->redirs = NULL;
	cmd->tokens = NULL;
	cmd->next = NULL;
	return (cmd);
}

/**
 * Creates a new redirection
 */
t_redir	*new_redir(t_token_type type, char *file)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = strdup(file);
	redir->fd = -1;
	redir->next = NULL;
	return (redir);
}

t_env_var	*new_var(char *key, char *value)
{
	t_env_var	*env;

	env = malloc(sizeof(t_env_var));
	if (!env)
		return (NULL);
	env->key = key;
	env->value = value;
	env->next = NULL;
	return (env);
}
