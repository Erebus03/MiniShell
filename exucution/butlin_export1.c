/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   butlin_export1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:33:19 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/01 18:35:20 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void execute_export(t_general *data)
{
	t_command * var ;
	var = data->cmnd ;
	

	if(var->cmd[0] && var->cmd[1] == NULL)
		afficher_exp(data->envlst);
	else
		process_all_export_args(data);
}

void afficher_exp(t_env_var *env)
{
  		t_env_var *temp ;
		temp = env ;
		while(temp)
		{
			printf("declare -x ");
			printf("%s",temp->key);
			if(temp->value != NULL)
				printf("=\"%s\"", temp->value);
			printf("\n");
			temp =temp ->next ;
		}
}

 void process_all_export_args(t_general *data)
{
	t_command *var = data->cmnd;
	size_t i = 1;

	while(var && i < ftt_strlen((const char **)var->cmd) && var->cmd[i])
	{
		if(chek_export(var->cmd[i]) == 0 )
		{
			eroor_export(var->cmd[i]);
			generale.exit_status = 0;
			return ;
		}
	
		if (!process_single_export_arg(data, var->cmd[i]))
		{
			generale.exit_status = 1;
			return;
		}
		i++;
	}
}

int chek_export(char *var)
{
    int i = 0;
    
    if (!var || !var[0])
        return 0;
    
     if (!((var[0] >= 'a' && var[0] <= 'z') || 
          (var[0] >= 'A' && var[0] <= 'Z') || 
          var[0] == '_'))
        return 0;
    
    i = 1;
    while(var[i] && var[i] != '=')
    {
        if(!((var[i] >= 'a' && var[i] <= 'z') || 
             (var[i] >= 'A' && var[i] <= 'Z') || 
             (var[i] >= '0' && var[i] <= '9') || 
             var[i] == '_'))
            return 0;
        i++;
    }
    return 1; 
}

 int process_single_export_arg(t_general *data, char *arg)
{
	char *key;
	int j = 0;

	while(arg[j] != '=' && arg[j] != '\0')
		j++;
	if(arg[j] == '\0')
	{
		key = extract_key(arg, j);
		if (!key)
			return 0;
		return handle_no_value_export(data, key);
	}
	else
		return handle_with_value_export(data, arg, j);
}
