/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fonction2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 22:15:35 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/13 22:46:44 by alamiri          ###   ########.fr       */
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
		generale.exit_status = 127;
		return (-1);
	}
	if (access(data->cmnd->cmd[0], X_OK) != 0)
	{
		print_error(data->cmnd->cmd[0], ": Permission denied\n");
		ft_freeee(data);
		generale.exit_status = 126;
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
	if (generale.cmnd)
		free_commands(&generale.cmnd);
	perror(msg);
	generale.exit_status = 1;
	exit(generale.exit_status);
}
