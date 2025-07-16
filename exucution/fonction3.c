/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fonction3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:10:36 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/17 00:47:35 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_app(t_general *data)
{
	if (data->input[0] == '\0')
		return (-1);
	if (ft_herdoc(data) == -1)
	{
		free(data->input);
		free_commands(&data->cmnd);
		cleanup(data);
		return (-1);
	}
	return (0);
}

void	app_bultin(void)
{
	int	j;
	int	fdi;
	int	fdo;

	j = 0;
	fdi = dup(STDIN_FILENO);
	fdo = dup(STDOUT_FILENO);
	j = chek_type(g_generale.cmnd->redirs, &g_generale);
	if (j != -1)
		aplementation_bultin(&g_generale);
	dup2(fdi, STDIN_FILENO);
	dup2(fdo, STDOUT_FILENO);
	return ;
}

void	ft_genereac(int ac, char **av)
{
	(void)av;
	if (ac != 1)
	{
		printf("$> ./Your Program\n");
		g_generale.exit_status = 1;
		exit(g_generale.exit_status);
	}
	return ;
}

void	ft_freemain(t_command *cmds)
{
	free(g_generale.input);
	free_commands(&cmds);
	cleanup(&g_generale);
}
