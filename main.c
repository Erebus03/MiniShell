/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:57:53 by araji             #+#    #+#             */
/*   Updated: 2025/07/16 22:18:33 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_general	generale;

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

int  ft_app(t_general *data)
{
	if(data->input[0] == '\0')
			return -1;
	if (ft_herdoc(data) == -1)
	{
		free(data->input);
		free_commands(&data->cmnd);
		cleanup(data);
		return -1;
	}
	return 0;
}

void app_bultin()
{
	int j= 0;
	int fd = dup(STDIN_FILENO);
	int k = dup(STDOUT_FILENO);
	j = chek_type(generale.cmnd->redirs,&generale);
	if (j != -1 )
		aplementation_bultin(&generale);
	dup2(fd,STDIN_FILENO);
	dup2(k,STDOUT_FILENO);
}
void ft_genereac(int ac)
{
	if (ac != 1)
	{
		printf("$> ./Your Program\n");
		generale.exit_status = 1 ;
		exit(generale.exit_status);
	}
}
void ft_freemain(t_command	*cmds)
{
	free(generale.input);
	free_commands(&cmds);
	cleanup(&generale);
}
int	main(int ac, char **av, char **envp)
{
	t_command	*(cmds);
	(void)av;
	ft_genereac(ac);
	init_general_struct(&generale, NULL);
	list_env_vars(&generale.envlst, envp);
	while (1)
	{
		signal(SIGINT, sighandler);
		signal(SIGQUIT, SIG_IGN);
		generale.input = readline("\001\033[32m\002minihell $> \001\033[0m\002");
		if (!generale.input)
			ft_control(&generale);
		if (generale.input[0])
			add_history(generale.input);
		cmds = parse_command(&generale);
		generale.cmnd = cmds;
		if(ft_app(&generale) == -1)
			continue;
		if (size_list(generale.cmnd) == 1 && chek_bultin(generale.cmnd) == 1)
			app_bultin();	
		else
			ft_exucutepipe(&generale);
		ft_freemain(cmds);
	}
	return (0);
}
