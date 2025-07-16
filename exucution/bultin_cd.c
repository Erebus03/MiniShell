/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultin_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:23:07 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/17 00:47:35 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handel_cdhome(t_general *data)
{
	t_env_var	*temp;

	temp = data->envlst;
	while (temp)
	{
		if (ft_strcmp(temp->key, "HOME") == 0)
		{
			if (chdir(temp->value) != 0)
			{
				eroor_cd(temp->value);
				g_generale.exit_status = 1;
			}
			return ;
		}
		temp = temp->next;
	}
	if (temp == NULL)
	{
		write(2, "bash: cd: HOME not set\n", 23);
		g_generale.exit_status = 1;
	}
	return ;
}

void	save_cd(t_general *data)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path != NULL)
	{
		free(data->pwd);
		data->pwd = ft_strdup(path);
	}
	free(path);
	return ;
}

void	execute_cd(t_general *data)
{
	char	*path;

	data->oldpwd = getcwd(NULL, 0);
	add_addr(data, new_addr(data->oldpwd));
	if (data->cmnd->cmd[0] && data->cmnd->cmd[1] == NULL)
		handel_cdhome(data);
	else if (chdir(data->cmnd->cmd[1]) == 0)
		save_cd(data);
	else
		return (eroor_cd(data->cmnd->cmd[1]));
	path = getcwd(NULL, 0);
	add_addr(data, new_addr(path));
	if (!path)
		errror_path(data);
	if (path != NULL)
	{
		edit_env(data->envlst, path);
		oldedit_env(data->envlst, data->oldpwd);
	}
	g_generale.exit_status = 0;
	return ;
}

void	errror_path(t_general *data)
{
	char	*jon;

	perror("cd: error retrieving current directory");
	g_generale.exit_status = 1;
	jon = ft_strjoin(data->pwd, "/..");
	if (data->pwd != NULL)
		free(data->pwd);
	data->pwd = ft_strdup(jon);
	add_addr(data, new_addr(jon));
	edit_env(data->envlst, jon);
	return ;
}

void	oldedit_env(t_env_var *var, char *newpath)
{
	t_env_var	*temp;

	temp = var;
	while (temp)
	{
		if (strncmp(temp->key, "OLDPWD", 6) == 0)
		{
			free(temp->value);
			temp->value = ft_strdup(newpath);
			return ;
		}
		temp = temp->next;
	}
	return ;
}
