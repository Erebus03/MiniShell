#include "../minishell.h"

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

void    add_variable(t_env_var **lst, t_env_var *new)
{
		t_env_var *tmp;
	
        if (!lst || !new)
                return ;
        if (!*lst)
        {
			// printf("first to be appended (%s=%s)[%p]\n", new->key, new->value, new);
			*lst = new;
			return ;
        }
    	tmp = *lst;
		// printf ("staring with %p(%s=%s)\n", tmp, tmp->key, tmp->value);
    	int i = 0;
		while (tmp->next)
		{
			// printf("passing (%%s=%%s)[%p]\n", tmp); //tmp->key, tmp->value, tmp);
        	tmp = tmp->next;
			i++;
		}
		// printf("skipped %d vars\nappended %s=%s\nto %s=%s\n",
		// 	i, new->key, new->value, tmp->key, tmp->value);
        tmp->next = new;
}

t_env_var* list_env_vars(t_env_var **envlst, char **envp)
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
		
		add_variable(envlst, variable);
		// write(1, "added one new variable\n\n", 25);
		i++;
	}
	return NULL;
}
