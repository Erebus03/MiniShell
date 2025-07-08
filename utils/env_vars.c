/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:58:07 by araji             #+#    #+#             */
/*   Updated: 2025/07/07 14:49:02 by araji            ###   ########.fr       */
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
		{
			if (tmp->value == NULL)
				return (NULL);
			return (ft_strdup(tmp->value));
		}
		tmp = tmp->next;
	}
	return (NULL);
}

char	*join_key_value(const char *key, const char *value)
{
	char	*result;

	int (keylen), (len);
	keylen = ft_strlen(key);
	len = keylen + ft_strlen(value) + 1;
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

static int	make_add_variable(t_env_var **envlst,
		char *key, char *val)
{
	t_env_var	*variable;

	variable = new_var(key, val);
	if (!variable)
		return (0);
	add_variable(envlst, variable);
	return (1);
}

/*
	   i  	=> indx[0]
	   j	=> indx[1]
	val_len => indx[2]
*/
void	list_env_vars(t_env_var **envlst, char **envp)
{
	int			indx[3];
	char		*key;
	char		*value;

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
		if (!make_add_variable(envlst, key, value))
			return ;
	}
}
