/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   genere_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:20:32 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/01 18:55:21 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_child_process(t_general *data, int *fd_sa, int *fd)
{
	t_redir	*cc;

	cc = data->cmnd->redirs;
	if (fd_sa[0] != -1)
	{
		close(fd_sa[1]);
		dup2(fd_sa[0], STDIN_FILENO);
		close(fd_sa[0]);
	}
	if (data->cmnd->next)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	chek_type(cc, data);
	if (data->cmnd->cmd[0] != NULL)
		split_chek(data);
	exit(generale.exit_status);
}

 void	handle_parent_process(int *fd_sa, int *fd, t_general *data)
{
	if (fd_sa[0] != -1)
	{
		close(fd_sa[0]);
		close(fd_sa[1]);
	}
	if (data->cmnd->next)
	{
		fd_sa[0] = fd[0];
		fd_sa[1] = fd[1];
	}
}

 
void	wait_all_processes(int *pids, int count)
{
	int	j;
	int	status;

	j = 0;
	while (j < count)
	{
		waitpid(pids[j], &status, 0);
		if (WIFEXITED(status))
			generale.exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			generale.exit_status = 128 + WTERMSIG(status);
		j++;
	}
}



int	*alloc_pids(t_general *data)
{
	int	*pids;

	pids = malloc(sizeof(int) * size_list(data->cmnd));
	if (!pids)
	{
		generale.exit_status = 1;
		exit(generale.exit_status);
	}
	return (pids);
}

void	ft_exucutepipe(t_general *data)
{
	int (pid), (i), (*pids), (fd_sa[2]), (fd[2]);
	fd_sa[0] = -1;
	fd_sa[1] = -1;
	i = 0;
	pids = alloc_pids(data);
	while (data->cmnd)
	{
		if (data->cmnd->next)
			pipe(fd);
		pid = fork();
		if (pid < 0)
			exit_error("fork");
		else if (pid == 0)
			handle_child_process(data, fd_sa, fd);
		else if (pid > 0)
		{
			pids[i++] = pid;
			handle_parent_process(fd_sa, fd, data);
		}
		data->cmnd = data->cmnd->next;
	}
	wait_all_processes(pids, i);
}