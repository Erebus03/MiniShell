/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultin.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:30:41 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/04 23:15:34 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void aplementation_bultin(t_general *data)
{
	if(ft_strcmp(data->cmnd->cmd[0],"cd")==0)
		execute_cd(data);
	else if(ft_strcmp(data->cmnd->cmd[0],"echo") ==0)
		execute_echo(data);
	else if(ft_strcmp(data->cmnd->cmd[0],"pwd") ==0)
		execute_pwd(data);
	else if(ft_strcmp(data->cmnd->cmd[0],"export") ==0)
		execute_export(data);
	else if(ft_strcmp(data->cmnd->cmd[0],"unset") ==0)
		execute_unset(data);
	else if(ft_strcmp(data->cmnd->cmd[0],"env") ==0)
		execute_env(data);
	else if(ft_strcmp(data->cmnd->cmd[0],"exit") ==0)
		execute_exit(data->cmnd);
	return ;
}

int  chek_bultin(t_command * var)
{
	int i ;
	char *bultin[]={"cd","pwd","echo","env","exit","export","unset",NULL};

	i= 0;
	while(bultin[i])
	{
		if(ft_strcmp(bultin[i],var->cmd[0]) == 0)
			return 1;
		i++;
	}
	return 0;
}