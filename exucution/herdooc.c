/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdooc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:34:22 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/17 01:33:26 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*namefile(t_general *data)
{
	char	*r;
	char	*name;
	char	*res;

	name = ttyname(STDIN_FILENO);
	r = "/tmp/herdoc_";
	res = ft_strjoin(r, name + 9);
	add_addr(data, new_addr(res));
	return (res);
}

int	child_herdoc(t_redir *var)
{
	char	*pop;

	while (1)
	{
		signal(SIGINT, heredoc_sigint_handler);
		pop = readline("> ");
		if (!pop || ft_strcmp(pop, var->file) == 0)
		{
			g_generale.exit_status = 0;
			close(var->fd);
			free(pop);
			return (1);
		}
		if (var->expand_in_heredoc == 1)
			expand_in_heredoc(var->fd, pop);
		else
			write(var->fd, pop, ft_strlen(pop));
		write(var->fd, "\n", 1);
		free(pop);
	}
	return (0);
}

int	parent_herdoc(int pid, t_redir *var)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, SIG_DFL);
	close(var->fd);
	if (WIFEXITED(status))
		g_generale.exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		g_generale.exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
		{
			unlink(var->index);
			g_generale.exit_status = 130;
			return (g_generale.exit_status);
		}
	}
	return (g_generale.exit_status);
}

int	herdocc(t_redir *var, int index, t_general *data)
{
	int		pid;
	char	*name;
	char	*s;

	name = namefile(data);
	s = ft_itoa(index);
	var->index = ft_strjoin(name, s);
	add_addr(data, new_addr(s));
	add_addr(data, new_addr(var->index));
	var->fd = open(var->index, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (var->fd < 0)
		return (perror("open"), -1);
	pid = fork();
	if (pid == 0)
	{
		child_herdoc(var);
		ft_freeeea(data);
		exit(data->exit_status);
	}
	else if (pid > 0)
		return (parent_herdoc(pid, var));
	else
		return (perror("fork"), close(var->fd), -1);
	return (g_generale.exit_status);
}

int	ft_herdoc(t_general *data)
{
	t_command	*temp;
	t_redir		*redir;
	int			index;

	temp = data->cmnd;
	index = 0;
	while (temp != NULL)
	{
		redir = temp->redirs;
		while (redir != NULL)
		{
			if (redir->type == THEREDOC)
				if (herdocc(redir, index, data) == 130)
					return (-1);
			redir = redir->next;
		}
		index++;
		temp = temp->next;
	}
	return (0);
}
