/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signale.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:42:43 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/17 00:47:35 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc_sigint_handler(int sig)
{
	(void) sig;
	free(g_generale.input);
	free_commands(&g_generale.cmnd);
	cleanup(&g_generale);
	free(g_generale.pwd);
	free_envp(&g_generale, 'b');
	clear_history();
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_redisplay();
	g_generale.exit_status = 130;
	exit(g_generale.exit_status);
}

void	sighandler(int sig)
{
	(void) sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_generale.exit_status = 130;
}

void	ft_control(t_general *data)
{
	free(data->pwd);
	free_envp(data, 'b');
	clear_history();
	write(1, "exit\n", 5);
	g_generale.exit_status = 0;
	exit(g_generale.exit_status);
}

void	child_sigint_handler(int sig)
{
	(void) sig ;
	write(1, "\n", 1);
	g_generale.exit_status = 130;
	exit(g_generale.exit_status);
}

void	child_sigquit_handler(int sig)
{
	(void) sig ;
	write(1, "\n", 1);
	g_generale.exit_status = 131;
	exit(g_generale.exit_status);
}
