/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:57:53 by araji             #+#    #+#             */
/*   Updated: 2025/07/05 22:21:31 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_general generale;

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
	context->inside_env_var = 0;
	context->pwd = NULL;
	context->oldpwd=NULL;
}

int	main(int ac, char **av, char **envp)
{
	t_general data ;
	t_command	*cmds;
	(void)av;
	
	init_general_struct(&data, NULL);
	if (ac != 1)
	{
		printf("$> ./Your Program\n");
		return 0 ;
	}
	list_env_vars(&data.envlst, envp);

	cleanup(&data);
	
	while (1)
	{
		signal(SIGINT, sighandler);
		signal(SIGQUIT, SIG_IGN);
		data.input = readline("\001\033[32m\002minihell $> \001\033[0m\002");
		add_history(data.input);
		if (!data.input)
			ft_control();
		if(*data.input == '\0')
			continue;
		cmds = parse_command(&data);
		t_command *var = cmds;
		data.cmnd = cmds;
		if (ft_herdoc(var) == -1)
			continue;
		if (size_list(var) == 1 && chek_bultin(var) == 1)
		{	int j= 0;
			int fd = dup(STDIN_FILENO);
			int k = dup(STDOUT_FILENO);
			j = chek_type(var->redirs,&data);
			if (j != -1 )
				aplementation_bultin(&data);
			dup2(fd,STDIN_FILENO);
			dup2(k,STDOUT_FILENO);
		}
		else
			ft_exucutepipe(&data);
		free(data.input);
		free_commands(&cmds);
		cleanup(&data);
	}
	return (0);
}


