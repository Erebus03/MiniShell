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

int main(int ac, char **av, char **envp)
{
	(void)av;
	t_general context;

	/* should init the general struct in a separat function (do later) */
	context.input = NULL;
	context.envlst = NULL;
	context.envarr = NULL;
	context.error = SUCCESS;
	context.error_msg = NULL;
	context.exit_status = 0;

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
	// copy_envp(&context);
	// for (int i = 0; context.envarr[i]; i++) {
	// 	printf("var(%d) = %s\n", i, context.envarr[i]);
	// }
	// write(1, "\nizfinish\n\n", 10);
	
	while(37)
	{
  		context.input = readline("\001\033[32m\002minihell $> \001\033[0m\002");
		if (ft_strncmp(context.input, "exit", 4) == 0)
				exit(0);
		if (validate_quotes(&context) == 0) // this "'"fffFFF"'" cleans to 'fffFFF' shit weird
			printf("SYNTAX ERROR: unclosed quotes\n");
  	  	printf("u entered '%s'\n", context.input);
		add_history(context.input);
		parse_command(&context);
	}
	return (0);
}
