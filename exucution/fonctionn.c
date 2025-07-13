/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fonctionn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:41:24 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/13 20:58:42 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ftt_strlen(const char **s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

int	size_list(t_command *var)
{
	int	i;

	i = 0;
	while (var != NULL)
	{
		i++;
		var = var->next;
	}
	return (i);
}

char	*getenvp(char **p)
{
	size_t	i;

	i = 0;
	while (p[i])
	{
		if (strncmp("PATH=", p[i], 5) == 0)
		{
			return (p[i]);
		}
		i++;
	}
	return (NULL);
}

char	*cherche_path(t_env_var **env)
{
	t_env_var	*var;

	var = *env;
	while (var)
	{
		if (strncmp(var->key, "PATH", 4) == 0)
			return (var->value);
		var = var->next;
	}
	return (NULL);
}

void	edit_env(t_env_var *var, char *newpath)
{
	t_env_var	*temp;

	temp = var;
	while (temp)
	{
		if (strncmp(temp->key, "PWD", 3) == 0)
		{
			free(temp->value);
			temp->value = ft_strdup(newpath);
			return ;
		}
		temp = temp->next;
	}
	return ;
}
