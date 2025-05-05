#include "minishell.h"

char **copy_envp(char **env)
{
	int i , vars, var_len;
	char **cpy;

	i = 0;
	vars = 0;
	//we first count how many env vars are there	
	while (env[i++] != NULL)
		vars++;

	cpy = malloc((vars + 1) * sizeof(char *));
	i = 0;
	while (env[i] != NULL)
	{
		var_len = ft_strlen(env[i]);
		cpy[i] = malloc(var_len + 1);
		ft_memcpy(cpy[i], env[i], var_len);
		cpy[i][var_len] = '\0';
		i++;
	}
	cpy[vars] = NULL;
	return cpy;
}

int main(int ac, char **av, char **envp)
{
	
	/* SOME GENERAL STRUCT DECLARED HERE */
	(void)av;
	t_general context;
	if (ac != 1)
	{
		printf("$> ./Your Program\n");
		exit(1);
	}
	else
		context.envp = copy_envp(envp);

	for (int i = 0; context.envp[i] != NULL ; i++) {
		printf(">%d : %s\n", i, context.envp[i]);
	}

	while(37)
	{
  		context.input = readline("\001\033[32m\002minihell $> \001\033[0m\002");
		if (ft_strncmp(context.input, "exit", 4) == 0)
				exit(0);
  	  	printf("u entered '%s'\n", context.input);
		add_history(context.input);
		// parse_cmd(lst, cmd);
	}
	/* free env_cpy */
	int i = 0;
	while (context.envp[i] != NULL)
		free(context.envp[i++]);
	free(context.envp);

	return (0);
}
