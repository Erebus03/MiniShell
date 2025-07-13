/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exucute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:14:44 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/13 22:49:32 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exucutecmd(char **env, char *path, t_general *data)
{
	t_command	*var;
	struct stat	info;

	var = data->cmnd;
	if (stat(path, &info) == 0 && S_ISDIR(info.st_mode))
	{
		print_error(path, ": Is a directory\n");
		ft_freeee(data);
		generale.exit_status = 126;
		exit(generale.exit_status);
	}
	else
	{
		execve(path, var->cmd, env);
		perror("Erooorrrr execve\n");
		ft_freeee(data);
		generale.exit_status = 127;
	}
	return ;
}

int	chek_eroorsplit(t_general *data)
{
	if (data->cmnd->cmd && data->cmnd->cmd[0] && data->cmnd->cmd[0][0] == '\0')
	{
		print_error(data->cmnd->cmd[0], ": command not found\n");
		ft_freeee(data);
		return (generale.exit_status = 127, -1);
	}
	if (data->cmnd->cmd && data->cmnd->cmd[0] && (data->cmnd->cmd[0][0] == '/'
		|| data->cmnd->cmd[0][0] == '.'))
	{
		if (access(data->cmnd->cmd[0], F_OK) != 0)
		{
			print_error(data->cmnd->cmd[0], ": No such file or directory\n");
			ft_freeee(data);
			return (generale.exit_status = 127, -1);
		}
		if (access(data->cmnd->cmd[0], X_OK) != 0)
		{
			print_error(data->cmnd->cmd[0], ": Permission denied\n");
			ft_freeee(data);
			return (generale.exit_status = 126, -1);
		}
		exucutecmd(data->envarr, data->cmnd->cmd[0], data);
	}
	return (1);
}

void	split_pathexucutecmd(char *path, t_general *data)
{
	int		i;
	char	**split_env;

	char *(strjonn), *(strjoncmd);
	i = 0;
	split_env = ft_split(path, ':');
	while (split_env[i] != NULL)
	{
		strjonn = ft_strjoin(split_env[i], "/");
		strjoncmd = ft_strjoin(strjonn, data->cmnd->cmd[0]);
		free(strjonn);
		if (access(strjoncmd, F_OK) == 0)
		{
			if (access(strjoncmd, X_OK) == 0)
			{
				exucutecmd(data->envarr, strjoncmd, data);
				return (free(strjoncmd));
			}
			else
				return (eroor_exucutecmn(strjoncmd));
		}
		free(strjoncmd);
		i++;
	}
	ft_free(split_env);
}

void	split_chek(t_general *data)
{
	char	*path;

	if (chek_bultin(data->cmnd) == 1)
	{
		aplementation_bultin(data);
		return ;
	}
	copy_envp(data);
	if (chek_eroorsplit(data) == -1)
		return ;
	path = cherche_path(&data->envlst);
	if (!path)
	{
		if (ft_chekl(data) == -1)
			return ;
		ft_freeee(data);
	}
	split_pathexucutecmd(path, data);
	print_error(data->cmnd->cmd[0], ": command not found\n");
	ft_freeee(data);
	generale.exit_status = 127;
	return ;
}

void	ft_freeee(t_general *data)
{
	free(data->input);
	free(data->pwd);
	free_commands(&data->cmnd);
	cleanup(data);
	free_envp(data, 'b');
	clear_history();
}
