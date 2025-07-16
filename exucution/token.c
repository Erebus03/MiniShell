/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:18:06 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/17 00:47:35 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_tokin_redir(t_redir *com, t_general *data)
{
	int	fd;

	fd = tokencomnd(com, data);
	if (fd < 0)
	{
		g_generale.exit_status = 1;
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("minishell: dup2");
		close(fd);
		g_generale.exit_status = 1;
		return (-1);
	}
	close(fd);
	return (fd);
}

int	ft_redir_append(t_redir *com, t_general *data)
{
	int	fd;

	fd = tokeappend(com, data);
	if (fd < 0)
	{
		g_generale.exit_status = 1;
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("minishell: dup2");
		close(fd);
		g_generale.exit_status = 1;
		return (-1);
	}
	close(fd);
	return (fd);
}

int	ft_redir_in(t_redir *com, t_general *data)
{
	int	fd;

	fd = tokeredir(com, data);
	if (fd < 0)
	{
		g_generale.exit_status = 1;
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("minishell: dup2");
		close(fd);
		g_generale.exit_status = 1;
		return (-1);
	}
	close(fd);
	return (fd);
}

int	ft_redir_herdoc(t_redir *com)
{
	int	fd;

	fd = open(com->index, O_RDONLY, 0644);
	if (fd < 0)
	{
		perror("minishell: heredoc");
		g_generale.exit_status = 1;
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("minishell: dup2");
		close(fd);
		g_generale.exit_status = 1;
		return (-1);
	}
	close(fd);
	return (fd);
}

int	chek_type(t_redir *cc, t_general *data)
{
	int	k;

	k = 6;
	while (cc)
	{
		if (cc->type == TREDIR_OUT)
			k = ft_tokin_redir(cc, data);
		else if (cc->type == TREDIR_APPEND)
			k = ft_redir_append(cc, data);
		else if (cc->type == TREDIR_IN)
			k = ft_redir_in(cc, data);
		else if (cc->type == THEREDOC)
			k = ft_redir_herdoc(cc);
		cc = cc->next;
	}
	return (k);
}
