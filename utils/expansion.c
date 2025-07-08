/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 21:41:16 by araji             #+#    #+#             */
/*   Updated: 2025/07/08 16:05:58 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	helper function 3la hsab norminette	*/
void	increment_val(int *value1, int *value2, int *value3, int *value4)
{
	if (value1)
		*value1 += 1;
	if (value2)
		*value2 += 1;
	if (value3)
		*value3 += 1;
	if (value4)
		*value4 += 1;
}

int	calculate_expansion_size(t_general *ctx, int i, char stop_char)
{
	int (result_size), (processed);
	char *(temp_value);
	result_size = 0;
	while (ctx->input[i] && ctx->input[i] != stop_char)
	{
		if (ctx->input[i] == '$' && stop_char != '\''
			&& ctx->no_expand_heredoc == 0)
		{
			processed = handle_dollar(ctx, i, &temp_value);
			if (processed <= 0)
				return (-1);
			if (temp_value)
			{
				result_size += ft_strlen(temp_value);
				free(temp_value);
			}
			else
				result_size++;
			i += processed;
		}
		else
			increment_val(&result_size, &i, NULL, NULL);
	}
	return (result_size);
}

void	fill_result_str(char *res, char *tmp_val, int *j)
{
	ft_strcpy(res + *j, tmp_val);
	*j += ft_strlen(tmp_val);
	free(tmp_val);
}

int	build_exp_str(t_general *ctx, int start, char stop_char, char *result)
{
	int (i), (j), (processed);
	char *(temp_value);
	i = start;
	j = 0;
	while (ctx->input[i] && ctx->input[i] != stop_char)
	{
		if (ctx->input[i] == '$' && stop_char != '\''
			&& ctx->no_expand_heredoc == 0)
		{
			processed = handle_dollar(ctx, i, &temp_value);
			if (processed <= 0)
				return (-1);
			if (temp_value)
				fill_result_str(result, temp_value, &j);
			else
				result[j++] = ctx->input[i++];
			i += processed;
		}
		else
			result[j++] = ctx->input[i++];
	}
	result[j] = '\0';
	return (i);
}

t_token	*split_token_value(char *value)
{
	t_general	tmp;
	t_token		*new_tokens;

	init_general_struct(&tmp, value);
	tmp.inside_env_var = 1;
	new_tokens = tokenize_input(&tmp);
	return (new_tokens);
}
