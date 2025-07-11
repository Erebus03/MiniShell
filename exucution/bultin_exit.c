/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultin_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:25:54 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/11 03:55:20 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int chek_number(char *s)
{
	int i = 0;

	if (!s || s[0] == '\0')
		return -1;
	if (s[0] == '+' || s[0] == '-')
		i++;
 	if (s[i] == '\0')
		return -1;
	while (s[i])
	{
		if (s[i] >= '0' && s[i] <= '9')
			i++;
		else 
			return -1;
	}
	return 1;
}

void singel_exit()
{
		write(1,"exit\n",5);
		generale.exit_status = 0;
		exit(generale.exit_status);
}


void exit_num_normal(int n)
{
	generale.exit_status = n ;
	exit(generale.exit_status);
}

void exit_num(int n)
{
		n = n % 256 ;
		generale.exit_status = n ;
		exit(generale.exit_status);
}

void execute_exit(t_command * var)
{
	int n ;

	if(ftt_strlen((const char **)var->cmd) == 1 && ft_strcmp(var->cmd[0],"exit") == 0)
		singel_exit();
	else if(ftt_strlen((const char **)var->cmd) == 2)
	{
		if(ft_strcmp(var->cmd[0],"exit") == 0 && chek_number(var->cmd[1]) == 1)
		{
			write(1,"exit\n",5);
			 n = ft_atoi(var->cmd[1]); /// atttention atoi type de return ;
			if(n >= 0 && n <= 255)
				exit_num_normal(n);
			else 
				exit_num_normal(n);
		}
		else if(ft_strcmp(var->cmd[0],"exit") == 0 && chek_number(var->cmd[1]) == -1)
		{
			generale.exit_status = 2;
			eroor_exit(var->cmd[1]);
			exit(generale.exit_status);
		}
	}
	else
		return(three_exit());
}
