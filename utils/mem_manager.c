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
/*
void	add_addr(t_general *ctx, t_memory *new_addr)
{
	t_memory	*tmp;

	if (!new_addr)
		return ;
	if (!ctx->heap)
	{
		ctx->heap = new_addr;
		return ;
	}
	tmp = ctx->heap;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_addr;
}

t_memory	*new_addr(void *ptr)
{
	t_memory	*node;

	node = malloc(sizeof(t_memory));
	if (!node)
	{
		// cleanup (ctx);
		return NULL;
	}
	node->ptr = ptr;
	node->next = NULL;
	return (node);
}

void	*allocate(t_general *ctx, size_t size)
{
	void *mem = malloc(size);

	if (!mem)
	{
		cleanup(ctx);
		return NULL;
	}
	add_addr(ctx, new_addr(mem));
	return mem;
}
*/

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
	(void)errmsg;
	// printf("%s\n", errmsg);
	write(1, "print failnt\n", 13);
	// cleanup(ctx);
	write(1, "cleaned uppp\n", 13);
	free_envp(ctx, 'b');
	write(1, "fred envm'b'\n", 13);
	exit(errcode);
}
