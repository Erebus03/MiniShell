/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:57:53 by araji             #+#    #+#             */
/*   Updated: 2025/06/22 19:56:48 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_general_struct(t_general *context, char *value)
{
	context->input = value;
	context->envlst = NULL;
	context->envarr = NULL;
	context->error = SUCCESS;
	context->heap = NULL;
	context->error_msg = NULL;
	context->exit_status = 0;
	context->no_expand_heredoc = 0;
}
void	run(void)
{
	return ;
}	
int	main(int ac, char **av, char **envp)
{
	t_general	(context);
	t_command	*cmds;
	(void)av;
	init_general_struct(&context, NULL);
	if (ac != 1)
	{
		printf("$> ./Your Program\n");
		exit(1);
	}
	list_env_vars(&context.envlst, envp);
	while (37)
	{
		context.input = readline("\001\033[32m\002minihell $> \001\033[0m\002");
		if (ft_strcmp(context.input, "exit") == 0)
		{
			write(1, "calling exit\n", 13);
			clean_exit(&context, NULL, 0);  //exit(0); // exit with 2
		}
		add_history(context.input);
		/**/cmds = parse_command(&context);
		run();
		// print_commands(cmds);
		free_commands(&cmds);
	}
	return (0);
}
