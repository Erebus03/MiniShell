/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exucute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:14:44 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/05 22:23:13 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"
char **reserve_space(t_general *data)
{
	t_command *var = data->cmnd ;
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

void exucutecmd(char **env, char *path,t_general *data)
{
	t_command *var  = data->cmnd;
	
	struct stat info;
	char		**argv;
	int i;
	
 	if (stat(path, &info) == 0 && S_ISDIR(info.st_mode)) 
	{
		print_error(path, ": Is a directory\n");
		generale.exit_status = 126;
		exit(generale.exit_status);
	}
	else
	{
		argv = reserve_space(data);
		data->heap = new_addr(argv);
		add_addr(data,data->heap);
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
			print_error(data->cmnd->cmd[0], ": command not found\n");
			generale.exit_status = 127;
			// cleanup(data);
			return -1;
	}
	if	(data->cmnd->cmd && data->cmnd->cmd[0] && (data->cmnd->cmd[0][0] =='/' || data->cmnd->cmd[0][0] == '.' ))
	{
		if (access(data->cmnd->cmd[0], F_OK) != 0)
		{
			print_error(data->cmnd->cmd[0], ": No such file or directory\n");
			generale.exit_status = 127;
			// cleanup(data);
			return -1 ;
		}
		if (access(data->cmnd->cmd[0], X_OK) != 0)
		{
			print_error(data->cmnd->cmd[0], ": Permission denied\n");
			generale.exit_status = 126;
			// cleanup(data);
			return -1 ;
		}
		exucutecmd(data->envarr, data->cmnd->cmd[0], data);
		return -1 ;
	}
	return 0;
}
void	ft_free(char **p)
{
	int	i;

	i = 0;
	while (p && p[i])
	{
		free(p[i]);
		i++;
	}
	free(p);
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
				exucutecmd(data->envarr, strjoncmd, data);
				free(strjoncmd);
				return ;
			}
			else
				return(eroor_exucutecmn(strjoncmd));
		}
		free(strjoncmd);
		i++;
	}
	ft_free(split_env);
}

// void ft_chekl(t_general *data)
// {
// 	if (access(data->cmnd->cmd[0], F_OK) != 0)
// 	{
// 		print_error(data->cmnd->cmd[0], ": No such file or directory\n");
// 		generale.exit_status = 127;
// 		return  ;
// 	}
// 	if (access(data->cmnd->cmd[0], X_OK) != 0)
// 	{
// 		print_error(data->cmnd->cmd[0], ": Permission denied\n");
// 		generale.exit_status = 126;
// 		return ;
// 	}
// 	exucutecmd(data->envarr, data->cmnd->cmd[0], data);
// }

void split_chek(t_general *data)
{
	char *path;

	if(chek_bultin(data->cmnd) == 1)
	{
		aplementation_bultin(data);
		return ;
	}
	copy_envp(data);
	// ft_chekl(data); ??
	if (chek_eroorsplit(data) == -1 )
		return ;
	path = cherche_path(&data->envlst);
	if (!path)
	{
		print_error(data->cmnd->cmd[0], ": command not found\n");
		generale.exit_status = 127;
		return;
	}
	split_pathexucutecmd(path,data);
	
	print_error(data->cmnd->cmd[0], ": command not found\n");	
	generale.exit_status= 127;
	return ;
		
}
