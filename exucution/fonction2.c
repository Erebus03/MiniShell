/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fonction2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 22:15:35 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/17 00:47:35 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free(char **p)
{
	int	i;

	i = 0;
	while (p && p[i])
	{
		free(p[i]);
		i++;
	}
	free(p);
}

int	ft_chekl(t_general *data)
{
	if (access(data->cmnd->cmd[0], F_OK) != 0)
	{
		print_error(data->cmnd->cmd[0], ": No such file or directory\n");
		ft_freeee(data);
		g_generale.exit_status = 127;
		return (-1);
	}
	if (access(data->cmnd->cmd[0], X_OK) != 0)
	{
		print_error(data->cmnd->cmd[0], ": Permission denied\n");
		ft_freeee(data);
		g_generale.exit_status = 126;
		return (-1);
	}
	exucutecmd(data->envarr, data->cmnd->cmd[0], data);
	return (0);
}

void	setup_child_signals(void)
{
	signal(SIGINT, child_sigint_handler);
	signal(SIGQUIT, child_sigquit_handler);
}

void	exit_error(char *msg)
{
	if (g_generale.cmnd)
		free_commands(&g_generale.cmnd);
	perror(msg);
	g_generale.exit_status = 1;
	exit(g_generale.exit_status);
}

void	init_general_struct(t_general *context, char *value)
{
	context->input = value;
	context->envlst = NULL;
	context->envarr = NULL;
	context->error = SUCCESS;
	context->heap = NULL;
	context->cmnd = NULL;
	context->error_msg = NULL;
	context->exit_status = 0;
	context->no_expand_heredoc = 0;
	context->inside_env_var = 0;
	context->pwd = NULL;
	context->oldpwd = NULL;
}
