/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultin_echo_pwd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:23:54 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/04 21:07:02 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int chek_newline(char * var)
{
	int i ;
	i = 1 ;
	while (var && var[i])
	{
		if(var[i] == 'n')
			i++;
		else	
			return (0);	
	}
	return 1;
}

void process_echo(t_general *data,int i)
{
	t_command *var;

	var = data->cmnd ;
	while(var && var->cmd[i] && var->cmd[i][0])
	{
		write(1,var->cmd[i],ft_strlen(var->cmd[i]));
		i++;
		if(var->cmd[i] != NULL)
			write(1," ",1);
	}
	return ;
}

void execute_echo(t_general *data)
{
	t_command * var = data->cmnd ;
	int i; 
	int res = -1;
	i = 1;	
	while(var && var->cmd[i]&& var->cmd[i][0])
	{
			if(var->cmd[i][0] == '-' && var->cmd[i][1] != '\0')
				{
					res = chek_newline(var->cmd[i]);
					if(res == 1)
						i++;
					else
						break;
				}
			else
						break;	
	}
	process_echo(data,i);
	if(res != 1)
		write(1,"\n",1);
	generale.exit_status = 0 ;
}


char *getpath(t_env_var **pp)
{
	t_env_var *p = *pp;

	while (p)
	{
		if (strncmp("PWD", p->key, 3) == 0)
			return p->value;
		p = p->next;
	}
	return (NULL);
}


void execute_pwd(t_general *data)
{
	(void)data;
	// char pwddd[10] = "/anahna";
	// char *p = getpath(&data->envlst);
	// if (p == NULL)
	// {
	// 	dprintf(2,"AAPWD======%s==========\n",p);
	
		char *d = getcwd(NULL, 0);
		if (d != NULL)
				data->pwd = ft_strdup(d);
		if (d == NULL)
		{
			// dprintf(2, "%s\n", pwddd);
			dprintf(2,"%s\n",data->pwd);
			// perror("Erooor pwd");
			generale.exit_status = 1;
			return ;
		}
		printf("%s\n", d);
		free(d);
		generale.exit_status = 0;
	// }
	// else
	// {
	// 	printf("%s\n",p);
	// 	generale.exit_status = 0;
	// }
	return ;
}
