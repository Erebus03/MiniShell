/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eroor_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:29:07 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/04 21:33:44 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void print_error(char * str, char *error_message) 
{
	write(2, "bash: ", 6);
	write(2, str, strlen(str));
	write(2, error_message, ft_strlen(error_message));	
}

int eroor_msg(t_general *data, int flag)
{
	char *str;

	str = data->cmnd->redirs->file ;
	write(2, "bash: ", 6);
	write(2, str, strlen(str));
	if (flag == 1)
		write(2, ": Is a directory\n", 18);
	else if (flag == 2)
		write(2, ": Permission denied\n", 21);
	else if (flag == 3)
		write(2, ": No such file or directory\n", 28);
	else if (flag == 4)
		write(2, ": command not found\n", 21);
	else if (flag == 5)
		write(2, ": ambiguous redirect\n", 21);

	generale.exit_status = 1;
	if (size_list(data->cmnd) == 1 && chek_bultin(data->cmnd) == 1)
		return -1;  
	else
		exit(generale.exit_status);
}
void eroor_export(char *str)
{
	write(2, "bash: export: ", 14);
	write(2, str, strlen(str));
	write(2, ": not a valid identifier\n", 26);
	generale.exit_status = 1;
	return ;
}
void eroor_exit(char *str)
{
	write(2,"exit\n",5);
	write(2, "bash: exit: ", 12);
	write(2, str, strlen(str));
	write(2, ": numeric argument required\n", 28);
	return ;
}

void eroor_cd(char *str)
{
	write(2, "bash: cd: ", 10);
	write(2, str, strlen(str));
	write(2, ": No such file or directory\n", 28);
	generale.exit_status = 1 ;
	return ;
}

void eroor_exucutecmn(char *strjoncmd)
{
	write(2, ": Permission denied\n", 21);
	free(strjoncmd);
	generale.exit_status = 126;
	return;
}