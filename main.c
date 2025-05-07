#include "minishell.h"

int validate_quotes(t_general *ctx)
{
    int i;
    char quote_char;
    
    i = 0;
    while (ctx->input[i])
    {
        if (ctx->input[i] != '"' && ctx->input[i] != '\'')
        {
            i++;
            continue;
        }
        quote_char = ctx->input[i++];
        while (ctx->input[i] && ctx->input[i] != quote_char)
        {
            if (quote_char == '"' && ctx->input[i] == '\\'
					&& ctx->input[i+1] && ctx->input[i+1] == '"')
                i += 2;
            else
                i++;
        }
        if (!ctx->input[i])
            return 0;
        i++;
    }
    return 1;
}

char *join_key_value(const char *key, const char *value) {
    int len = strlen(key) + strlen(value) + 2; // +1 for '=' and +1 for '\0'
    char *result = malloc(len);
    if (!result) return NULL;
    ft_strcpy(result, key);
    ft_strcat(result, "=");
    ft_strcat(result, value);
    return result;
}
char	**copy_envp(t_env_var *lst)
{
	t_env_var *tmp;
	char **envp;
	char *key;
	char *value;
	char *varf;
	int i;

	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	envp = malloc((i + 1) * sizeof(char *));
	i = 0;
	while (lst)
	{
		join_key_value(lst->key, lst->value);
	}

}

int main(int ac, char **av, char **envp)
{
	
	(void)av;
	t_general context;

	if (ac != 1)
	{
		printf("$> ./Your Program\n");
		exit(1);
	}
	context.envlst = list_env_vars(&context.envlst, envp);
	while(37)
	{
  		context.input = readline("\001\033[32m\002minihell $> \001\033[0m\002");
		if (ft_strncmp(context.input, "exit", 4) == 0)
				exit(0);
		if (validate_quotes(&context) == 0) // this "'"fffFFF"'" cleans to 'fffFFF'
			printf("SYNTAX ERROR: unclosed quotes\n");
  	  	printf("u entered '%s'\n", context.input);
		add_history(context.input);
		// parse_cmd(lst, cmd);
	}

	return (0);
}
