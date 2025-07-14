/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   genere_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:32:27 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/14 01:49:00 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	tokencomnd(t_redir *cout, t_general *data)
{
	struct stat	info;
	int			fd;

	if (ft_strchr(cout->file, ' '))
		return (eroor_msg(data, 5));
	if (stat(cout->file, &info) == 0 && S_ISDIR(info.st_mode))
		return (eroor_msg(data, 1));
	fd = open(cout->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (eroor_msg(data, 2));
	generale.exit_status = 0;
	return (fd);
}

int	tokeredir(t_redir *cout, t_general *data)
{
	struct stat	info;
	int			fd;

	if (ft_strchr(cout->file, ' '))
		return (eroor_msg(data, 5));
	if (access(cout->file, F_OK) == -1)
		return (eroor_msg(data, 3));
	if (stat(cout->file, &info) == 0 && S_ISDIR(info.st_mode))
		return (eroor_msg(data, 1));
	fd = open(cout->file, O_RDONLY, 0644);
	if (fd < 0)
		return (eroor_msg(data, 2));
	generale.exit_status = 0;
	return (fd);
}

int	tokeappend(t_redir *cout, t_general *data)
{
	struct stat	info;
	int			fd;

	if (ft_strchr(cout->file, ' '))
		return (eroor_msg(data, 5));
	if (stat(cout->file, &info) == 0 && S_ISDIR(info.st_mode))
		return (eroor_msg(data, 1));
	fd = open(cout->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0)
		return (eroor_msg(data, 2));
	generale.exit_status = 0;
	return (fd);
}
