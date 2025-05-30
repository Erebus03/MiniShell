/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 20:04:30 by araji             #+#    #+#             */
/*   Updated: 2025/05/30 02:38:53 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_addr(t_general *ctx, t_memory *new_addr)
{
	t_memory	*tmp;

	if (!ctx->mem_manager)
	{
		ctx->mem_manager = new_addr;
		return ;
	}
	tmp = ctx->mem_manager;
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
		cleanup (ctx, 1 , "Memory allocation failed : new_addr()");
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
		cleanup (ctx, 1 , "Memory allocation failed : allocate()");
		return NULL;
	}

	add_addr(ctx, new_addr(mem));
	return mem;
}

void	cleanup(t_general *ctx, int exit_status, char *msg_err)
{
	t_memory *tmp;
	printf("CLEANING UP CUZ : %s\n", msg_err);

	while (ctx->mem_manager)
	{
		tmp = ctx->mem_manager;
		ctx->mem_manager = ctx->mem_manager->next;
		
		printf("freeing %p\n", tmp->ptr);
		free(tmp->ptr);
		tmp->ptr = NULL;
		free(tmp);
		tmp = NULL;

	}
	ctx->mem_manager = NULL;
	ctx->exit_status = exit_status;
}
