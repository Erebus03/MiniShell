#include "minishell.h"

int main(int ac, char **av)
{
	/* SOME GENERAL STRUCT DECLARED HERE */
	// node *lst = NULL;
	if (ac != 1)
	{
		printf("$> ./Your Program\n");
		return 1;
	}
	while(37)
	{
 		char *cmd;
    	cmd = readline("\001\033[32m\002minihell $> \001\033[0m\002");
    	printf("u entered '%s'\n", cmd);
		add_history(cmd);
		// parse_cmd(lst, cmd);
	}
    return (0);
}