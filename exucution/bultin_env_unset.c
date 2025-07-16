/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultin_env_unset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:27:41 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/17 00:47:35 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	int			i;
	t_command	*temp;

	temp = data->cmnd;
	i = 1;
	while (temp && temp->cmd && temp->cmd[i])
	{
		remove_env_var(data, temp->cmd[i]);
		i++;
	}
	g_generale.exit_status = 0;
	return ;
}

char	**initialse_data(t_general *data)
{
	int		i;
	char	*pwd;
	char	**p;

	pwd = getcwd(NULL, 0);
	data->oldpwd = getcwd(NULL, 0);
	p = malloc(sizeof(char *) * 5);
	if (!p)
		return (NULL);
	i = 0;
	p[0] = ft_strjoin("PWD=", pwd);
	p[1] = ft_strjoin("SHLVL=", "1");
	p[2] = ft_strjoin("_=", "/minishell");
	p[3] = ft_strjoin("OLDPWD=", data->oldpwd);
	p[4] = NULL;
	while (p && p[i])
	{
		add_addr(data, new_addr(p[i]));
		i++;
	}
	add_addr(data, new_addr(p));
	free(pwd);
	free(data->oldpwd);
	return (p);
}

void	execute_env(t_general *env)
{
	t_env_var	*var;

	var = env->envlst;
	if (var == NULL)
	{
		afficher_envv(env);
		return ;
	}
	var = env->envlst;
	while (var != NULL)
	{
		if (var->key != NULL && var->value != NULL)
		{
			printf("%s", var->key);
			if (var->value != NULL)
				printf("=%s", var->value);
			printf("\n");
		}
		var = var->next;
	}
	g_generale.exit_status = 0;
	return ;
}

void	afficher_envv(t_general *envv)
{
	t_env_var	*var;
	char		**res;

	res = initialse_data(envv);
	list_env_vars(&envv->envlst, res);
	var = envv->envlst;
	while (var != NULL)
	{
		if (ft_strcmp(var->key, "OLDPWD") == 0)
			var = var->next;
		else
		{
			printf("%s", var->key);
			printf("=%s", var->value);
			printf("\n");
			var = var->next;
		}
	}
	g_generale.exit_status = 0;
	return ;
}
