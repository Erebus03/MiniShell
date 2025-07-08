/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 20:04:30 by araji             #+#    #+#             */
/*   Updated: 2025/06/14 16:50:02 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_addr(t_general *ctx, t_memory *new_addr)
{
	t_memory	*tmp;

	if (!new_addr)
	{
		cleanup(ctx);
		return (0);
	}
	if (!ctx->heap)
	{
		ctx->heap = new_addr;
		return (1);
	}
	tmp = ctx->heap;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_addr;
	return (1);
}

t_memory	*new_addr(void *ptr)
{
	t_memory	*node;

	node = malloc(sizeof(t_memory));
	if (!node)
		return (NULL);
	node->ptr = ptr;
	node->next = NULL;
	return (node);
}

void	cleanup(t_general *ctx)
{
	t_memory	*tmp;

	while (ctx->heap)
	{
		tmp = ctx->heap;
		ctx->heap = ctx->heap->next;
		free(tmp->ptr);
		tmp->ptr = NULL;
		free(tmp);
		tmp = NULL;
	}
	ctx->heap = NULL;
}

void	clean_exit(t_general *ctx, char *errmsg, int errcode)
{
	if (errmsg)
		printf("%s\n", errmsg);
	cleanup(ctx);
	free_envp(ctx, 'b');
	free_commands(&ctx->cmnd);
	generale.exit_status = errcode;
}
