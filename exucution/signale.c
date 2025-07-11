/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signale.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:42:43 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/11 04:43:20 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void heredoc_sigint_handler()
{
    write(1, "\n", 1);         
	rl_replace_line("", 0);
    rl_redisplay();
    generale.exit_status = 130;
    exit(generale.exit_status);                 
}

void sighandler()
{
	write(1, "\n", 1);           
	rl_on_new_line();            
	rl_replace_line("", 0);     
	rl_redisplay();
	generale.exit_status = 130;
}

void ft_control(t_general *data)
{
	(void)data;
	// free(data->pwd);
	// if(data->cmnd  !=NULL)
	// 	free_commands(&data->cmnd);
	// cleanup(data);
	write(1,"exit\n",5);
	generale.exit_status = 0;
	exit(generale.exit_status);
}