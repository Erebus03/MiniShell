/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:57:53 by araji             #+#    #+#             */
/*   Updated: 2025/07/17 00:51:34 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_general	g_generale;

int	main(int ac, char **av, char **envp)
{
	t_command *(cmds);
	ft_genereac(ac, av);
	init_general_struct(&g_generale, NULL);
	list_env_vars(&g_generale.envlst, envp);
	while (1)
	{
		signal(SIGINT, sighandler);
		signal(SIGQUIT, SIG_IGN);
		g_generale.input = readline("minihell $> ");
		if (!g_generale.input)
			ft_control(&g_generale);
		if (g_generale.input[0])
			add_history(g_generale.input);
		cmds = parse_command(&g_generale);
		g_generale.cmnd = cmds;
		if (ft_app(&g_generale) == -1)
			continue ;
		if (size_list(g_generale.cmnd) == 1
			&& chek_bultin(g_generale.cmnd) == 1)
			app_bultin();
		else
			ft_exucutepipe(&g_generale);
		ft_freemain(cmds);
	}
	return (0);
}
