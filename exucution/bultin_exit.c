/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultin_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:25:54 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/13 22:45:56 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	singel_exit(void)
{
	free(generale.input);
	free_commands(&generale.cmnd);
	cleanup(&generale);
	free(generale.pwd);
	free_envp(&generale, 'b');
	clear_history();
	write(1, "exit\n", 5);
	generale.exit_status = 0;
	exit(generale.exit_status);
}

void	exit_num_normal(int n)
{
	free(generale.input);
	free_commands(&generale.cmnd);
	cleanup(&generale);
	free(generale.pwd);
	free_envp(&generale, 'b');
	clear_history();
	generale.exit_status = n;
	exit(generale.exit_status);
}

void	exit_num(int n)
{
	free(generale.input);
	free_commands(&generale.cmnd);
	cleanup(&generale);
	free(generale.pwd);
	free_envp(&generale, 'b');
	clear_history();
	n = n % 256;
	generale.exit_status = n;
	exit(generale.exit_status);
}

void	ft_generenumbre(t_command *var)
{
	int	n;

	write(1, "exit\n", 5);
	n = ft_atoi(var->cmd[1]);
	if (n >= 0 && n <= 255)
		exit_num_normal(n);
	else
		exit_num(n);
}

void	execute_exit(t_command *var)
{
	if (ftt_strlen((const char **)var->cmd) == 1 && ft_strcmp(var->cmd[0],
			"exit") == 0)
		singel_exit();
	else if (ftt_strlen((const char **)var->cmd) == 2)
	{
		if (ft_strcmp(var->cmd[0], "exit") == 0
			&& chek_number(var->cmd[1]) == 1)
			ft_generenumbre(var);
		else if (ft_strcmp(var->cmd[0], "exit") == 0
			&& chek_number(var->cmd[1]) == -1)
		{
			generale.exit_status = 2;
			eroor_exit(var->cmd[1]);
			exit(generale.exit_status);
		}
	}
	else if (ftt_strlen((const char **)var->cmd) > 2
		&& chek_number(var->cmd[1]) == 1)
	{
		write(2, "exit\n", 5);
		write(2, "bash: exit: too many arguments\n", 31);
		generale.exit_status = 1;
	}
	else
		return (three_exit());
}
