/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signale.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:42:43 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/14 00:25:54 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc_sigint_handler(void)
{
	free(generale.input);
	free_commands(&generale.cmnd);
	cleanup(&generale);
	free(generale.pwd);
	free_envp(&generale, 'b');
	clear_history();
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_redisplay();
	generale.exit_status = 130;
	exit(generale.exit_status);
}

void	sighandler(void)
{
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	generale.exit_status = 130;
}

void	ft_control(t_general *data)
{
	free(data->pwd);
	free_envp(data, 'b');
	clear_history();
	write(1, "exit\n", 5);
	generale.exit_status = 0;
	exit(generale.exit_status);
}

void	child_sigint_handler(int sig)
{
	(void) sig ;
	write(1, "\n", 1);
	generale.exit_status = 130;
	exit(generale.exit_status);
}

void	child_sigquit_handler(int sig)
{
	(void) sig ;
	write(1, "\n", 1);
	generale.exit_status = 131;
	exit(generale.exit_status);
}
