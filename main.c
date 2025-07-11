/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:57:53 by araji             #+#    #+#             */
/*   Updated: 2025/07/11 04:45:06 by alamiri          ###   ########.fr       */
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
	context->cmnd = NULL;
	context->error_msg = NULL;
	context->exit_status = 0;
	context->no_expand_heredoc = 0;
	context->inside_env_var = 0;
	context->pwd = NULL;
	context->oldpwd=NULL;
}

int	main(int ac, char **av, char **envp)
{
	// t_general data ;
	t_command	*cmds;
	(void)av;
	
	init_general_struct(&generale, NULL);
	if (ac != 1)
	{
		printf("$> ./Your Program\n");
		return 0 ;
	}
	list_env_vars(&generale.envlst, envp);

	while (1)
	{
		signal(SIGINT, sighandler);
		signal(SIGQUIT, SIG_IGN);
		generale.input = readline("\001\033[32m\002minihell $> \001\033[0m\002");
		add_history(generale.input);
		if (!generale.input)
			ft_control(&generale);
		if(generale.input[0] == '\0')
			continue;
		cmds = parse_command(&generale);
		generale.cmnd = cmds;
		generale.cmnddd = cmds;
		if (ft_herdoc(&generale) == -1)
		{
		free(generale.input);
		free_commands(&cmds);
		cleanup(&generale);
		continue;
		}
		if (size_list(generale.cmnd) == 1 && chek_bultin(generale.cmnd) == 1)
		{	int j= 0;
			int fd = dup(STDIN_FILENO);
			int k = dup(STDOUT_FILENO);
			j = chek_type(generale.cmnd->redirs,&generale);
			if (j != -1 )
				aplementation_bultin(&generale);
			dup2(fd,STDIN_FILENO);
			dup2(k,STDOUT_FILENO);
		}
		else
			ft_exucutepipe(&generale);
		
		free(generale.input);
		free_commands(&cmds);
		cleanup(&generale);
	}
	return (0);
}