/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultin.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:30:41 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/17 00:47:35 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	aplementation_bultin(t_general *data)
{
	if (ft_strcmp(data->cmnd->cmd[0], "cd") == 0)
		execute_cd(data);
	else if (ft_strcmp(data->cmnd->cmd[0], "echo") == 0)
		execute_echo(data);
	else if (ft_strcmp(data->cmnd->cmd[0], "pwd") == 0)
		execute_pwd(data);
	else if (ft_strcmp(data->cmnd->cmd[0], "export") == 0)
		execute_export(data);
	else if (ft_strcmp(data->cmnd->cmd[0], "unset") == 0)
		execute_unset(data);
	else if (ft_strcmp(data->cmnd->cmd[0], "env") == 0)
		execute_env(data);
	else if (ft_strcmp(data->cmnd->cmd[0], "exit") == 0)
		execute_exit(data->cmnd);
	return ;
}

int	chek_bultin(t_command *var)
{
	int		i;
	char	*bultin[8];

	bultin[0] = "cd";
	bultin[1] = "pwd";
	bultin[2] = "echo";
	bultin[3] = "env";
	bultin[4] = "exit";
	bultin[5] = "export";
	bultin[6] = "unset";
	bultin[7] = NULL;
	i = 0;
	while (bultin[i])
	{
		if (ft_strcmp(bultin[i], var->cmd[0]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	chek_doublenumber(char **s)
{
	int	i;
	int	j;

	i = 1;
	if (!s || s[1][0] == '\0')
		return (-1);
	if (s[1][0] == '+' || s[1][0] == '-')
		i++;
	if (s[1][0] == '\0')
		return (-1);
	while (s[i])
	{
		j = 0;
		while (s[i][j])
		{
			if (s[i][j] >= '0' && s[i][j] <= '9')
				j++;
			else
				return (-1);
		}
		i++;
	}
	return (1);
}

void	free_env_var(t_env_var *env_var)
{
	free(env_var->key);
	free(env_var->value);
	free(env_var);
}

void	three_exit(void)
{
	free(g_generale.input);
	free_commands(&g_generale.cmnd);
	cleanup(&g_generale);
	free(g_generale.pwd);
	free_envp(&g_generale, 'b');
	clear_history();
	write(2, "exit\n", 5);
	write(2, "bash: exit: numeric argument required\n", 38);
	g_generale.exit_status = 1;
	exit(g_generale.exit_status);
	return ;
}
