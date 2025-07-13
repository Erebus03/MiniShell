/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdooc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:34:22 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/14 00:21:44 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *namefile(t_general *data)
{
	char *r;
	char *name ;
	name = ttyname(STDIN_FILENO);
	r ="/tmp/herdoc_";
	char *res = ft_strjoin(r,name+9);
     add_addr(data,new_addr(res));
	return res ;
}

static int	write_exit_status(int fd)
{
	char	*value;

	value = ft_itoa(generale.exit_status);
	write(fd, value, ft_strlen(value));
	free(value);
	return (2);
}

static int	write_env_value(int fd, char *str, int index)
{
	char	*value;
	char	*name;
	int		dollar_pos;
	int		var_len;

	value = NULL;
	dollar_pos = index++;
	var_len = 0;
	while (str[index] && (ft_isalnum(str[index]) || str[index] == '_'))
		increment_val(&index, &var_len, NULL, NULL);
	// printf("******\nstr %s\n**********\n", str + dollar_pos);
	name = (char *)malloc(var_len + 1); //free
	if (!name)
		return 0;
	ft_memcpy(name, str + dollar_pos + 1, var_len);
	name[var_len] = '\0';

	// printf("******\nvarname = %s\n**********\n", name);
	
	if (var_len > 0)
	{
		value = get_env_value(name, generale.envlst);
		// printf("getenvvalue ret %s\n", value);
	}
	else if (str[index] != '\0')
		value = NULL;
	free(name);
	if (value)
		write(fd, value, ft_strlen(value));
	free(value);
	// printf("returning %d\n", index - dollar_pos);
	return (index - dollar_pos);
}

static void	expand_in_heredoc(int fd, char *str)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (str[i])
	{
		if (str[i] != '$')
			ret = write(fd, &str[i], 1);
		else if (str[i + 1] == '$')
		{
			int pid = getpid();
			ft_putnbr_fd(pid, fd);
			ret += 2;
		}
		else if (str[i + 1] == '?')
			ret += write_exit_status(fd);
		else if (str[i + 1] == '\0')
			ret = write(fd, "$", 1);
		else
			ret = write_env_value(fd , str, i);
		i += ret;
	}
}

void child_herdoc(t_redir *var)
{
	char *pop;		 
           
	while (1)
	{
        signal(SIGINT, heredoc_sigint_handler); 
	    signal(SIGQUIT, SIG_IGN);  
        signal(SIGINT, SIG_DFL);
		pop = readline("> ");
		if (!pop || ft_strcmp(pop, var->file) == 0)  
		{
			generale.exit_status = 0;
			close(var->fd);     
			exit(generale.exit_status);           
		}
        if (var->expand_in_heredoc == 1)
			expand_in_heredoc(var->fd, pop);
		else
			write(var->fd, pop, ft_strlen(pop));
		write(var->fd, "\n", 1);
		free(pop);
	}
    free(generale.input);
	free_commands(&generale.cmnd);
	cleanup(&generale);
	free(generale.pwd);
	free_envp(&generale, 'b');
	clear_history();
	return ;
}

int parent_herdoc(int pid,t_redir *var)
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
                unlink(var->index);
				generale.exit_status = 130;
                return (generale.exit_status);      
            }
        }
        return generale.exit_status;
}

int herdocc(t_redir *var, int index,t_general *data)
{
    int pid;
	char (*name),(*s);
	// char *s;
    // var->index =NULL ;
    name = namefile(data);
	s = ft_itoa(index);
	var->index = ft_strjoin(name,s);
    add_addr(data,new_addr(s));
    add_addr(data,new_addr(var->index));
    var->fd = open(var->index, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (var->fd < 0)
        return (perror("open"),-1);
    pid = fork();
    if (pid == 0)
		child_herdoc(var);
    else if (pid > 0)
		return (parent_herdoc(pid,var));
    else
    {
        perror("fork");
        close(var->fd);
        return -1;
    }
    return generale.exit_status;
}

int ft_herdoc(t_general *data)
{
    t_command *temp  = data->cmnd ;
    t_redir *redir ;
	int index = 0;
    while (temp != NULL)
    {
        redir = temp->redirs;
        while(redir != NULL)
        {
            if (redir->type == THEREDOC)
                if (herdocc(redir,index,data) == 130)
					return (-1);
            redir = redir->next;
        }
		index ++;
        temp = temp->next;
    }
	return (0) ;
}

