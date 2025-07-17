/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expende_hedoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:02:02 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/17 04:00:54 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	write_exit_status(int fd)
{
	char	*value;

	value = ft_itoa(g_generale.exit_status);
	write(fd, value, ft_strlen(value));
	free(value);
	return (2);
}

static void	write_env_value_helper(char *name, char *value, int fd)
{
	if (value)
	{
		write(fd, value, ft_strlen(value));
		free(value);
	}
	free(name);
}

static int	write_env_value(int fd, char *str, int index)
{
	char *(value);
	char *(name);
	int (dollar_pos);
	int (var_len);
	value = NULL;
	dollar_pos = index++;
	var_len = 0;
	while (str[index] && (ft_isalnum(str[index]) || str[index] == '_'))
		increment_val(&index, &var_len, NULL, NULL);
	name = (char *)malloc(var_len + 1);
	if (!name)
		return (0);
	ft_memcpy(name, str + dollar_pos + 1, var_len);
	name[var_len] = '\0';
	if (var_len > 0)
		value = get_env_value(name, g_generale.envlst);
	else if ((str[index] != '\0' && str[index] != ' ')
		|| (dollar_pos > 0 && str[dollar_pos - 1] != '\0'
			&& str[dollar_pos - 1] != ' '))
		value = NULL;
	else
		value = ft_strdup("$");
	write_env_value_helper(name, value, fd);
	return (index - dollar_pos);
}

void	expand_in_heredoc(int fd, char *str)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (str[i])
	{
		if (str[i] != '$')
			ret = write(fd, &str[i], 1);
		else if (str[i + 1] == '?')
			ret += write_exit_status(fd);
		else
			ret = write_env_value(fd, str, i);
		i += ret;
	}
}

void	ft_freeeea(t_general *data)
{
	free(data->input);
	free(data->pwd);
	free_commands(&data->cmnd);
	cleanup(data);
	free_envp(data, 'b');
	clear_history();
}
