/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultin_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:23:07 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/08 16:55:35 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void  handel_cdhome(t_general *data)
{
	t_env_var *temp ; 
	temp = data->envlst;
	while (temp)
	{
		if(ft_strcmp(temp->key,"HOME") == 0)
		{
			if(chdir(temp->value) !=0)
			{
				free_envp(data, 'a');
				free_commands(&data->cmnd);
				cleanup(data);
				eroor_cd(temp->value);
				generale.exit_status = 1 ;
			}
			return ;
		}
		temp = temp ->next;
	}
	if(temp == NULL)
	{
		write(2,"bash: cd: HOME not set\n",23);
		generale.exit_status = 1 ;
	}
	return ;
}

void execute_cd(t_general *data)
{
	char *path;
	char *jon ;

	data->oldpwd = getcwd(NULL,0);
	add_addr(data, new_addr(data->oldpwd));
	if (data->cmnd->cmd[0] && data->cmnd->cmd[1] == NULL)
		handel_cdhome(data);
	else if(chdir(data->cmnd->cmd[1])!= 0)
		eroor_cd(data->cmnd->cmd[1]);
	generale.exit_status = 0;
	path = getcwd(NULL,0);
	add_addr(data, new_addr(path));
	if (!path)
	{	
		perror("cd: error retrieving current directory");
		generale.exit_status = 1;
		jon = ft_strjoin(data->pwd,"/..");
		add_addr(data, new_addr(jon));
		edit_env(data->envlst,jon);
	}
	if (path != NULL)
	{
		edit_env(data->envlst,path);
		oldedit_env(data->envlst,data->oldpwd);
	}
	return ;
}

void	exit_error(char *msg)
{
	perror(msg);
	generale.exit_status = 1;
	exit(generale.exit_status);
}

void oldedit_env(t_env_var *var,char *newpath)
{
	t_env_var *temp = var ;
	while(temp)
	{
		if(strncmp(temp->key,"OLDPWD",6) == 0)
		{
			temp->value = newpath ;
			return ;
		}
		temp= temp->next ;		
	}
	return ;
}