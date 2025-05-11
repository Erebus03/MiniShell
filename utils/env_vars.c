/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:58:07 by araji             #+#    #+#             */
/*   Updated: 2025/05/11 10:58:07 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* didnt work it yet */
char *get_env_value(char *var_name, char **envp)
{
    int i = 0;
    int var_len = ft_strlen(var_name);
    
    while (envp[i])
    {
        if (strncmp(envp[i], var_name, var_len) == 0 && envp[i][var_len] == '=')
            return strdup(envp[i] + var_len + 1);
        i++;
    }
    return strdup("");	// change later
}

t_env_var *new_var(char *key, char *value)
{
	t_env_var* env;

	env = malloc(sizeof(t_env_var));
	if (!env)
		return NULL;
	env->key = key;
	env->value = value;
	env->next = NULL;
	return env;
}

char *join_key_value(const char *key, const char *value)
{
    int keylen = ft_strlen(key);
    int len = keylen + ft_strlen(value) + 1;// +1 for '='
    char *result = malloc(len + 1);
    if (!result)
		return NULL;
    if (!ft_memcpy(result, key, keylen))
		return NULL;
    if (!ft_memcpy(result + keylen, "=", 1))
		return NULL;
    if (!ft_memcpy(result + keylen + 1, value, ft_strlen(value)))
		return NULL;
    result[len] = '\0';

    return result;
}
void	copy_envp(t_general *ctx)
{
	t_env_var *tmp = NULL;
	int i;

	i = 0;
	tmp = ctx->envlst;

	//get lst size
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	//allocate for the array
	ctx->envarr = malloc((i + 1) * sizeof(char *));
	
	
	i = 0;
	tmp = ctx->envlst;
	//for every variable, join key + '=' + value 
	while (tmp)
	{
		ctx->envarr[i++] = join_key_value(tmp->key, tmp->value);
		tmp = tmp->next;
	}
}

// Add a variable to the environment list
void add_variable(t_env_var **lst, t_env_var *new_var)
{
    t_env_var *tmp;

    if (!lst || !new_var) {
        return;
    }
    
    if (*lst == NULL) {
        printf("List is empty, setting head to %p\n", (void*)new_var);
        *lst = new_var;
        return;
    }
    
    tmp = *lst;
    while (tmp->next) {
        tmp = tmp->next;
    }
    
    printf("Adding %p at end of list after %p\n", (void*)new_var, (void*)tmp);
    tmp->next = new_var;
}

void	list_env_vars(t_env_var **envlst, char **envp)
{
	int (i), (j), (val_len);
	char *key;
	char *value;
	t_env_var* variable;

	i = 0;

	while (envp[i] != NULL)
	{
		j = 0;
		while (envp[i][j] != '=')
			j++;
		key = malloc(j + 1);
		if (!key)
			exit(1);
		ft_memcpy(key, envp[i], j);
		key[j++] = '\0';
		val_len = ft_strlen(envp[i]) - j;
		value = malloc(val_len + 1);
		if (!value)
			exit(1);
		ft_memcpy(value, envp[i] + j, val_len);
		value[val_len] = '\0';

		variable = new_var(key, value);
		if (!variable)
			exit(1);
		// printf("%s<-\n", envp[i]);
		// printf("kkk=%s\nvvv=%s<-\n", key, value);
		// printf("key=%s\nval=%s<-\n", variable->key, variable->value);
		// printf("\njj\n%p\n%p\n", envlst, variable);
		
		add_variable(envlst, variable);
		// write(1, "added one new variable\n\n", 25);
		i++;
	}
	// write(1, "jjjjd one new variable\n\n", 25);
	// printf("lst(%p) head(%p)\n", envlst, *envlst);
}
