/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eroor_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:29:07 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/17 00:47:35 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_error(char *str, char *error_message)
{
	write(2, "bash: ", 6);
	write(2, str, strlen(str));
	write(2, error_message, ft_strlen(error_message));
	return ;
}

int	eroor_msg(t_general *data, int flag)
{
	char	*str;

	str = data->cmnd->redirs->file;
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
	g_generale.exit_status = 1;
	if (size_list(data->cmnd) == 1 && chek_bultin(data->cmnd) == 1)
		return (-1);
	else
	{
		ft_freeee(data);
		exit(g_generale.exit_status);
	}
}

void	eroor_export(char *str, t_general *data)
{
	(void)data;
	write(2, "bash: export: ", 14);
	write(2, str, strlen(str));
	write(2, ": not a valid identifier\n", 26);
	g_generale.exit_status = 1;
	return ;
}

void	eroor_exit(char *str)
{
	write(2, "exit\n", 5);
	write(2, "bash: exit: ", 12);
	write(2, str, strlen(str));
	write(2, ": numeric argument required\n", 28);
	free(g_generale.input);
	free_commands(&g_generale.cmnd);
	cleanup(&g_generale);
	free(g_generale.pwd);
	free_envp(&g_generale, 'b');
	clear_history();
	return ;
}

void	eroor_cd(char *str)
{
	int	fd;

	fd = open(str, O_RDONLY);
	if (fd > 0)
		write(2, "bash: cd: ", 10);
	write(2, str, strlen(str));
	if (fd > 0)
	{
		write(2, " Is not a directory\n", 20);
		close(fd);
	}
	else
		write(2, ": No such file or directory\n", 28);
	g_generale.exit_status = 1;
	return ;
}
