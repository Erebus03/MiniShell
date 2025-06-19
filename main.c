/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:57:53 by araji             #+#    #+#             */
/*   Updated: 2025/06/19 21:29:44 by araji            ###   ########.fr       */
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

int	main(int ac, char **av, char **envp)
{
	t_general	(context);
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
			exit(0); // exit with 2
		// if (check_syntax(&context) == 0)
		// {
		// 	printf("syntax error\n");
		// 	add_history(context.input);
		// 	// cleanup(&context);
		// 	continue;
		// }
		add_history(context.input);
		parse_command(&context);
	}
	return (0);
}
