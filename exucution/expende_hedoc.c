/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expende_hedoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:02:02 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/16 22:04:45 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	name = (char *)malloc(var_len + 1);
	if (!name)
		return (0);
	ft_memcpy(name, str + dollar_pos + 1, var_len);
	name[var_len] = '\0';
	if (var_len > 0)
		value = get_env_value(name, generale.envlst);
	else if (str[index] != '\0')
		value = NULL;
	free(name);
	if (value)
		write(fd, value, ft_strlen(value));
	free(value);
	return (index - dollar_pos);
}

static void	expand_in_heredoc(int fd, char *str)
{
	int	i;
	int	ret;
	int	pid;

	i = 0;
	ret = 0;
	while (str[i])
	{
		if (str[i] != '$')
			ret = write(fd, &str[i], 1);
		else if (str[i + 1] == '$')
		{
			pid = getpid();
			ft_putnbr_fd(pid, fd);
			ret += 2;
		}
		else if (str[i + 1] == '?')
			ret += write_exit_status(fd);
		else if (str[i + 1] == '\0')
			ret = write(fd, "$", 1);
		else
			ret = write_env_value(fd, str, i);
		i += ret;
	}
}
