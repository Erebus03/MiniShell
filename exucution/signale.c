/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signale.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:42:43 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/01 18:44:10 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void heredoc_sigint_handler()
{
	rl_replace_line("", 0);
    rl_redisplay();
    generale.exit_status = 130;
    write(1, "\n", 1);         
    exit(generale.exit_status);                 
}

void sighandler()
{
	generale.exit_status = 130;
	write(1, "\n", 1);           
	rl_on_new_line();            
	rl_replace_line("", 0);     
	rl_redisplay();
}

void ft_control()
{
	write(1,"exit\n",5);
	generale.exit_status = 0;
	exit(generale.exit_status);
}