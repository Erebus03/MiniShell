/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdooc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:34:22 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/05 17:56:18 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



char *namefile()
{
	char *r;
	char *name ;
	name = ttyname(STDIN_FILENO);
	r ="/tmp/herdoc_";
	char *res = ft_strjoin(r,name+9);
	return res ;
}

void child_herdoc(t_redir *var)
{
	char *pop;		 
           
	while (1)
	{
        signal(SIGINT, heredoc_sigint_handler); 
	    signal(SIGQUIT, SIG_IGN);  
		pop = readline("> ");
		if (!pop || ft_strcmp(pop, var->file) == 0)  
		{
			generale.exit_status = 0;
			close(var->fd);     
			exit(generale.exit_status);           
		}
        // if(var->file mafihax l quotes)
        // pop = expande herd(char *str, ctx);
		write(var->fd, pop, ft_strlen(pop));
		write(var->fd, "\n", 1);
		free(pop);
	}
	return ;
}

int parent_herdoc(int pid,t_redir *var,char * name)
{
	  int status;

		signal(SIGINT, SIG_IGN);
        waitpid(pid, &status, 0);
		signal(SIGINT, SIG_DFL);
        close(var->fd);
        if (WIFEXITED(status))
            generale.exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
        {
            generale.exit_status = 128 + WTERMSIG(status); 
            if (WTERMSIG(status) == SIGINT)
            {
                unlink(name);
				generale.exit_status = 130;
                return (generale.exit_status);      
            }
        }
        return generale.exit_status;
}

int herdocc(t_redir *var, int index)
{
    int pid;
	char *name;
	char *s;

    name = namefile();
	s = ft_itoa(index);
	var->index = ft_strjoin(name,s);
    var->fd = open(var->index, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (var->fd < 0)
    {
        perror("open");
        return -1;  
    }
    pid = fork();
    if (pid == 0)
		child_herdoc(var);
    else if (pid > 0)
		return (parent_herdoc(pid,var,var->index));
    else
    {
        perror("fork");
        close(var->fd);
        return -1;
    }
    return generale.exit_status;
}

int ft_herdoc(t_command *commands)
{
    t_command *temp = commands;
	int index = 0;
    while (temp != NULL)
    {
        t_redir *redir = temp->redirs;
        while(redir != NULL)
        {
            if (redir->type == THEREDOC)
                if (herdocc(redir,index) == 130)
					return (-1);
            redir = redir->next;
        }
		index ++;
        temp = temp->next;
    }
	return (0) ;
}