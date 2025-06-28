/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:58:07 by araji             #+#    #+#             */
/*   Updated: 2025/06/28 14:10:56 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_value(char *var_name, t_env_var *lst)
{
	t_env_var *(tmp);
	tmp = lst;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, var_name) == 0)
			return (ft_strdup(tmp->value));
		tmp = tmp->next;
	}
	return (NULL); // change later
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
	ctx->envarr[i] = NULL;
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
	int	indx[3]; // i => indx[0] j => indx[1] val_len => indx[2]
	t_env_var *(variable);
	char *(key), *(value);
	indx[0] = -1;
	while (envp[++(indx[0])] != NULL)
	{
		indx[1] = 0;
		while (envp[indx[0]][indx[1]] != '=')
			(indx[1])++;
		key = malloc(indx[1] + 1);
		if (!key)
			return ;
		ft_memcpy(key, envp[indx[0]], indx[1]);
		key[(indx[1])++] = '\0';
		indx[2] = ft_strlen(envp[indx[0]]) - indx[1];
		value = malloc((indx[2]) + 1);
		if (!value)
			return ;
		ft_memcpy(value, envp[indx[0]] + indx[1], indx[2]);
		value[indx[2]] = '\0';
		variable = new_var(key, value);
		if (!variable)
			return ;
		add_variable(envlst, variable);
	}
}
