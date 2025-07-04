/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   butlin_export2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:36:27 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/04 21:09:33 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *extract_key(char *cmd_arg, int j)
{
    char *key = malloc(j + 1);
    if (!key)
        {
         generale.exit_status =1;   
         return NULL;
        }
    ft_memcpy(key, cmd_arg, j);
    key[j] = '\0';
    return key;
}

 int handle_no_value_export(t_general *data, char *key)
{
	char *value = NULL;

	if(!variable_exists(data->envlst, key))
		return handle_value_var(data, key, value);
	return 1;
}

int variable_exists(t_env_var *env_list, char *key)
{
    while (env_list)
    {
        if (ft_strcmp(key, env_list->key) == 0)
            {
				return 1;
				}
        env_list = env_list->next;
    }
    return 0;
}

int handle_value_var(t_general *data, char *key, char *value)
{
    t_env_var *variable;
    
   
        variable = new_var(key, value);
        if (!variable)
        {
            generale.exit_status = 1;
            return 0;
        }
        add_variable(&data->envlst, variable);
    return 1;
}


 int handle_with_value_export(t_general *data, char *arg, int j)
{
	t_env_var *variable = data->envlst;
	char *key, *value;

	key = extract_key(arg, j);
	if (!key)
		return 0;
	j++;
	value = extract_value(arg, j);
	if (!value)
		return 0;
	if(chek_exp(&data->envlst, key) == 1)
		update_existing_variable(variable, key, value);
	else
		return handle_value_var(data, key, value);
	return 1;
}
