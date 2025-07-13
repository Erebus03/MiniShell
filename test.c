/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:23:07 by alamiri           #+#    #+#             */
/*   Updated: 2025/07/13 22:09:35 by alamiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	chek_type(t_redir *cc, t_general *data)
{
	int	k;

	k = 6;
	cc = data->cmnd->redirs;
	while (cc)
	{
		if (cc->type == TREDIR_OUT)
			k = ft_tokin_redir(cc, data);
		else if (cc->type == TREDIR_APPEND)
			k = ft_redir_append(cc, data);
		else if (cc->type == TREDIR_IN)
			k = ft_redir_in(cc, data);
		else if (cc->type == THEREDOC)
			k = ft_redir_herdoc(cc);
		cc = cc->next;
	}
	return (k);
}
