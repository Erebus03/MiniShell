/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   butlin_export3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:38:48 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/11 03:55:51 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *extract_value(char *cmd_arg, int start_pos)
{
    int value_len = ft_strlen(cmd_arg) - start_pos;
    char *value = malloc(value_len + 1);
    if (!value)
        return NULL;
    ft_memcpy(value, cmd_arg + start_pos, value_len);
    value[value_len] = '\0';
    return value;
}

int  chek_exp(t_env_var **var,char *key)
{
	t_env_var *env;
	env = *var ;

	if(!key)
		return (-1) ;
	while(env)
	{
		if(ft_strcmp(key,env->key)== 0)
		{
			return 1;
		}
		env =env ->next;
	}
	return 0;
}

void update_existing_variable(t_env_var *env_list, char *key, char *value)
{
    while (env_list)
    {
        if (ft_strcmp(key, env_list->key) == 0)
        {
            env_list->value = value;
            break;
        }
        env_list = env_list->next;
    }
}

