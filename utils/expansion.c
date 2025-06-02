/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 21:41:16 by araji             #+#    #+#             */
/*   Updated: 2025/06/02 12:56:15 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	calculate_expansion_size(t_general *ctx, int i, char stop_char) //	i = start index
{
	int		(result_size), (processed);
	char	*(temp_value);
	result_size = 0;
	while (ctx->input[i] && ctx->input[i] != stop_char)
	{
		if (ctx->input[i] == '$' && stop_char != '\'' && ctx->no_expand_heredoc == 0)
		{
			// printf("calculating size with ctx->no_expand_heredoc = %d", ctx->no_expand_heredoc);
			processed = handle_dollar(ctx, i, &temp_value);
			if (processed <= 0)	// error handling
				return (-1);
			if (temp_value)
			{
				result_size += ft_strlen(temp_value);
				free(temp_value);
			}
			i += processed;
		}
		else
		{
			result_size++;
			i++;
		}
	}
	return (result_size);
}

int	build_expanded_string(t_general *ctx, int start, char stop_char, char *result)
{
	int		i, j, processed;
	char	*temp_value;

	i = start;
	j = 0;
	while (ctx->input[i] && ctx->input[i] != stop_char)
	{
		if (ctx->input[i] == '$' && stop_char != '\'' && ctx->no_expand_heredoc == 0)
		{
			// printf("proccessing char [%c] with ctx->no_expand_heredoc == %d\n", ctx->input[i], ctx->no_expand_heredoc);
			processed = handle_dollar(ctx, i, &temp_value);
			// split the input into tokens if the var has white spaces
			// link the tokens in a separate function
			// prossed the noraml work;
			if (processed <= 0)
				return (-1);
			if (temp_value)
			{
				strcpy(result + j, temp_value);	// build func
				j += ft_strlen(temp_value);
				free(temp_value);
			}
			i += processed;
		}
		else
			result[j++] = ctx->input[i++];
	}
	result[j] = '\0';

	printf("\nvalue we got from built_expanded_string [%s]\n\n", result);
	return (i);
}

t_token	*split_token_value(char *value)//, int *skipped)
{
	t_general	tmp;
	init_general_struct(&tmp, value);

	t_token		*new_tokens = tokenize_input(&tmp);

	// printf("split tokens\n");
	// print_tokens(new_tokens);
	// printf("out\n\n");


	// // ill create a general, put the value as original inpt
	// tokenize normally how i would tokenize the normal input
	// free it, link it
	return new_tokens;
}
