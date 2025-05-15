/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:58:07 by araji             #+#    #+#             */
/*   Updated: 2025/05/13 15:53:19 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* didnt work with it yet */
char	*get_env_value(char *var_name, t_env_var *lst)
{
	t_env_var *(tmp);
	int (var_len);
	var_len = ft_strlen(var_name);
	tmp = lst;
	while (tmp)
	{
		if (ft_strncmp(tmp->key, var_name, var_len) == 0)
			return (ft_strdup(tmp->value));
		tmp = tmp->next;
	}
	return (ft_strdup("")); // change later
}

char	*join_key_value(const char *key, const char *value)
{
	char	*result;

	int (keylen), (len);
	keylen = ft_strlen(key);
	len = keylen + ft_strlen(value) + 1;// +1 for '='
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	if (!ft_memcpy(result, key, keylen))
		return (NULL);
	if (!ft_memcpy(result + keylen, "=", 1))
		return (NULL);
	if (!ft_memcpy(result + keylen + 1, value, ft_strlen(value)))
		return (NULL);
	result[len] = '\0';
	return (result);
}

void	copy_envp(t_general *ctx)
{
	t_env_var	*tmp;
	int			i;

	tmp = NULL;
	i = 0;
	tmp = ctx->envlst;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	ctx->envarr = malloc((i + 1) * sizeof(char *));
	i = 0;
	tmp = ctx->envlst;
	while (tmp)
	{
		ctx->envarr[i++] = join_key_value(tmp->key, tmp->value);
		tmp = tmp->next;
	}
}

void	add_variable(t_env_var **lst, t_env_var *new_var)
{
	t_env_var	*tmp;

	if (!lst || !new_var)
		return ;
	if (*lst == NULL)
	{
		*lst = new_var;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_var;
}

void	list_env_vars(t_env_var **envlst, char **envp)
{
	t_env_var *(variable);
	char *(key), *(value);
	int (i), (j), (val_len);
	i = -1;
	while (envp[++i] != NULL)
	{
		j = 0;
		while (envp[i][j] != '=')
			j++;
		key = malloc(j + 1);
		if (!key) //cleanup()
			exit(1);
		ft_memcpy(key, envp[i], j);
		key[j++] = '\0';
		val_len = ft_strlen(envp[i]) - j;
		value = malloc(val_len + 1);
		if (!value) //cleanup()
			exit(1);
		ft_memcpy(value, envp[i] + j, val_len);
		value[val_len] = '\0';
		variable = new_var(key, value);
		if (!variable) //cleanup()
			exit(1);
		add_variable(envlst, variable);
	}
}
