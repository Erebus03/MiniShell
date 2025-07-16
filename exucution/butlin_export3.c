/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   butlin_export3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:38:48 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/17 00:47:35 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*extract_value(char *cmd_arg, int start_pos)
{
	int		value_len;
	char	*value;

	value_len = ft_strlen(cmd_arg) - start_pos;
	value = malloc(value_len + 1);
	if (!value)
		return (NULL);
	ft_memcpy(value, cmd_arg + start_pos, value_len);
	value[value_len] = '\0';
	return (value);
}

int	chek_exp(t_env_var **var, char *key)
{
	t_env_var	*env;

	env = *var;
	if (!key)
		return (-1);
	while (env)
	{
		if (ft_strcmp(key, env->key) == 0)
		{
			return (1);
		}
		env = env->next;
	}
	return (0);
}

void	update_existing_variable(t_env_var *env_list, char *key, char *value)
{
	char	*temp ;

	while (env_list)
	{
		if (ft_strcmp(key, env_list->key) == 0)
		{
			free(key);
			temp = env_list->value ;
			env_list->value = value;
			free(temp);
			break ;
		}
		env_list = env_list->next;
	}
}

int	chek_number(char *s)
{
	int	i;

	i = 0;
	if (!s || s[0] == '\0')
		return (-1);
	if (s[0] == '+' || s[0] == '-')
		i++;
	if (s[i] == '\0')
		return (-1);
	while (s[i])
	{
		if (s[i] >= '0' && s[i] <= '9')
			i++;
		else
			return (-1);
	}
	return (1);
}

void	eroor_exucutecmn(char *strjoncmd)
{
	write(2, ": Permission denied\n", 21);
	free(strjoncmd);
	g_generale.exit_status = 126;
	return ;
}
