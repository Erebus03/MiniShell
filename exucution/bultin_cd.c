/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultin_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:23:07 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/02 19:55:44 by alamiri          ###   ########.fr       */
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
	char *p=NULL;
	char *jon ;
	// t_env_var * temp = data->envlst ;
		
	if (data->cmnd->cmd[0] && data->cmnd->cmd[1] == NULL)
		handel_cdhome(data);
	else if(chdir(data->cmnd->cmd[1])!= 0)
	{
		eroor_cd(data->cmnd->cmd[1]);
        generale.exit_status = 1 ;
	}
	generale.exit_status = 0;
	path = getcwd(NULL,0);
	
	if (!path)
	{	
		perror("cd: error retrieving current directory");
		p = getpath(&data->envlst);
		jon =ft_strjoin(p,"/..");
		edit_env(data->envlst,jon);
	}
	if (path != NULL)
		edit_env(data->envlst,path);
	
		
		
	return ;
}

void	exit_error(char *msg)
{
	perror(msg);
	generale.exit_status = 1;
	exit(generale.exit_status);
}