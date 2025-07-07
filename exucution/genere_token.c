/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   genere_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:32:27 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/04 22:09:53 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int tokencomnd(t_redir *cout, t_general *data)
{
	struct stat info;
	int fd ;

 	if (stat(cout->file, &info) == 0 && S_ISDIR(info.st_mode)) 
    	{
		// eroor_msg(cout->file,1);
		return eroor_msg(data,1);
        // generale.exit_status = 1;
        // if (size_list(data->cmnd) == 1 && chek_bultin(data->cmnd) == 1)
        //     return -1;  
        // else
        //     exit(generale.exit_status);
   	 	}
	if(ft_strchr(cout->file,' ')) 
	{ // hna khassek t3ref wach smiya kan 3ndha "" ola la
		// eroor_msg(cout->file,5);
        // generale.exit_status = 1;
        // if (size_list(data->cmnd) == 1 && chek_bultin(data->cmnd) == 1)
        //     return -1;  
        // else
        //     exit(generale.exit_status);
   	 	// }
		return eroor_msg(data,5);
	}
 	fd = open(cout->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0) 
	{
		return eroor_msg(data,2);
		// eroor_msg(cout->file,2);
		// generale.exit_status = 1;
		// if (size_list(data->cmnd) == 1 && chek_bultin(data->cmnd) == 1)
		// 	return -1;
		// else
		// 	exit(generale.exit_status);
	}
	generale.exit_status=0;
	return fd;
}

// int handel_tokeredir(t_redir *cout,t_general *data,int flag)
// {
// 	 eroor_msg(cout->file,flag);
// 	 generale.exit_status = 1;
// 	 if (size_list(data->cmnd) == 1 && chek_bultin(data->cmnd) == 1)
//             return -1;  
//     else
//         exit(generale.exit_status);
// }

int tokeredir(t_redir *cout,t_general *data)
{
    struct stat info;
    int fd ;
    if (access(cout->file, F_OK) == -1) 
		return eroor_msg(data,3);
    if (stat(cout->file, &info) == 0 && S_ISDIR(info.st_mode)) 
		return eroor_msg(data,1);
	if(ft_strchr(cout->file,' ')) 
	{ 
		return eroor_msg(data,5);
	}
    fd = open(cout->file, O_RDONLY, 0644);
    if (fd < 0)
		return eroor_msg(data,2);
    generale.exit_status=0;
    return fd;
}

int tokeappend(t_redir *cout,t_general *data)
{
	struct stat info;
	int fd ;

	if (stat(cout->file, &info) == 0 && S_ISDIR(info.st_mode)) 
	{
		return eroor_msg(data,1);
		// eroor_msg(cout->file,1);
		// generale.exit_status = 1;
		// if (size_list(data->cmnd) == 1 && chek_bultin(data->cmnd) == 1)
		// 	return -1;  
		// else
		// 	exit(generale.exit_status);
	}
	if(ft_strchr(cout->file,' ')) 
	{ 
		return eroor_msg(data,5);
	}
	fd = open(cout->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0)
	{
		return eroor_msg(data,2);
	}
	generale.exit_status=0;
	return fd;
}

