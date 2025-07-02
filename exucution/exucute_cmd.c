/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exucute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:14:44 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/01 18:18:17 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"
char **reserve_space(t_command *var)
{
	int tail;
	char		**argv;
	tail = ftt_strlen((const char **)var->cmd);
	argv = malloc(sizeof(char *) * (tail + 1));
		if(!argv)
		{
			generale.exit_status=1;
			exit(generale.exit_status);	
		}
	return argv ;
}

void exucutecmd(char **env, char *path,t_command *var)
{
	struct stat info;
	char		**argv;
	int i;
	
 	if (stat(path, &info) == 0 && S_ISDIR(info.st_mode)) 
	{
	eroor_msg(path,1);
	generale.exit_status = 126;
	exit(generale.exit_status);
	}
	else
	{
		argv = reserve_space(var);
		i = 0;
		while (var->cmd[i])
		{
			argv[i] = ft_strdup(var->cmd[i]);
			i++;
		}
		argv[i] = NULL;
		execve(path, argv,env);
		perror("Erooorrrr execve\n");
		generale.exit_status=127;
		}
}



int  chek_eroorsplit(t_general *data)
{
	if (data->cmnd->cmd && data->cmnd->cmd[0] && data->cmnd->cmd[0][0] == '\0') 
	{
			eroor_msg(data->cmnd->cmd[0],4);
			generale.exit_status = 127;
			return -1;
	}
	if	(data->cmnd->cmd && data->cmnd->cmd[0] && (data->cmnd->cmd[0][0] =='/' || data->cmnd->cmd[0][0] == '.' ))
	{
		if (access(data->cmnd->cmd[0], F_OK) != 0)
		{
			eroor_msg(data->cmnd->cmd[0],3);
			generale.exit_status = 127;
			return -1 ;
		}
		if (access(data->cmnd->cmd[0], X_OK) != 0)
		{
			eroor_msg(data->cmnd->cmd[0],2);
			generale.exit_status = 126;
			return -1 ;
		}
		exucutecmd(data->envarr, data->cmnd->cmd[0], data->cmnd);
		return -1 ;
	}
	return 0;
}


void split_pathexucutecmd(char *path,t_general *data)
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
				exucutecmd(data->envarr, strjoncmd, data->cmnd);
				free(strjoncmd);
				return ;
			}
			else
				return(eroor_exucutecmn(strjoncmd));
		}
		free(strjoncmd);
		i++;
	}
}
void split_chek(t_general *data)
{
	char *path;

	if(chek_bultin(data->cmnd) == 1)
	{
		aplementation_bultin(data);
		return ;
	}
	copy_envp(data);
	if (chek_eroorsplit(data) == -1 )
		return ;
	path = cherche_path(&data->envlst);
	if (!path)
	{
		eroor_msg(data->cmnd->cmd[0],4);
		generale.exit_status = 127;
		return;
	}
	split_pathexucutecmd(path,data);
	eroor_msg(data->cmnd->cmd[0],4);	
	generale.exit_status= 127;
	return ;
}
