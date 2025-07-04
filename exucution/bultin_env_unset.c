/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultin_env_unset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:27:41 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/04 23:19:04 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

 void	free_env_var(t_env_var *env_var)
{
	free(env_var->key);
	free(env_var->value);
	free(env_var);
}

void	remove_env_var(t_general *data, char *var_name)
{
	t_env_var	*dor;
	t_env_var	*edit;

	edit = NULL;
	dor = data->envlst;
	if (dor && ft_strcmp(var_name, dor->key) == 0)
	{
		edit = dor;
		data->envlst = dor->next;
		free_env_var(edit);
	}
	else
	{
		while (dor)
		{
			if (dor->next && ft_strcmp(var_name, dor->next->key) == 0)
			{
				edit = dor->next;
				dor->next = edit->next;
				free_env_var(edit);
				break ;
			}
			dor = dor->next;
		}
	}
}

void	execute_unset(t_general *data)
{
	int i;
	t_command *temp;

	temp = data->cmnd;

	i = 1;
	while (temp && temp->cmd && temp->cmd[i])
	{
		remove_env_var(data, temp->cmd[i]);
		i++;
	}
	generale.exit_status = 0;
	return ;
}


void execute_env(t_general *env)
{
	t_env_var  *var = env->envlst;
	if(var == NULL)
	{
		char *path ;

		path = getcwd(NULL,0);
		printf("PWD=%s\n",path);
		printf("SHLVL=1\n");
		printf("_=./minishell\n");
		if(env->oldpwd != NULL)
			printf("OLDPWD=%s\n",env->oldpwd);
	}
	while(var !=  NULL)
	{
		
		if(var->key != NULL && var->value != NULL)
		{
		printf("%s",var->key);
		if(var->value != NULL)
				printf("=%s",var->value);
		printf("\n");
		}
		var = var->next;
	}
	generale.exit_status = 0;
	return ;
}

void three_exit()
{
	write(2,"exit\n",5);
	write(2,"bash: exit: too many arguments\n",31);
	generale.exit_status = 1;
	return ;
}