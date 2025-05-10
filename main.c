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

char *join_key_value(const char *key, const char *value)
{
    int keylen = ft_strlen(key);
    int len = keylen + ft_strlen(value) + 1; // +1 for '=' and +1 for '\0'
    char *result = malloc(len + 1);
    if (!result)
	return NULL;
    ft_memcpy(result, key, keylen);
    ft_memcpy(result + keylen, "=", 1);
    ft_memcpy(result + keylen + 1, value, ft_strlen(value));
    result[len] = '\0';

    return result;
}
void	copy_envp(t_general *ctx)
{
	t_env_var *tmp = NULL;
	//char *key;
	//char *value;
	//char *varf;
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

int main(int ac, char **av, char **envp)
{
	
	(void)av;
	t_general context;

	context.envlst = NULL;
	context.envarr = NULL;
	if (ac != 1)
	{
		printf("$> ./Your Program\n");
		exit(1);
	}
	list_env_vars(&context.envlst, envp);

	// t_env_var *tmp = context.envlst;
	// while (tmp) {
	// 	printf("(%p) [%s=%s]\n", tmp, tmp->key, tmp->value);
	// 	tmp = tmp->next;
	// }

	// function to make a 2D array of envp list, store in context.envarr
	copy_envp(&context);
	for (int i = 0; context.envarr[i]; i++) {
		printf("var(%d) = %s\n", i, context.envarr[i]);
	}
	write(1, "\nizfinish\n\n", 10);
	
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
