#include "../minishell.h"

/**
 * Split tokens into commands based on pipe operators
 */

t_command	*parse_commands(t_general *ctx)
{
    t_token *all_tokens = tokenize_input(ctx);
    t_command *commands = NULL;
    t_token *cmd_start = all_tokens;
    t_token *current = all_tokens;
    t_token *prev = NULL;
    t_command *new_cmd;
    
    if (!all_tokens)
        return NULL;
    
    while (current)
    {
        if (current->type == TOKEN_PIPE)
        {
            // Terminate the current command
            if (prev)
                prev->next = NULL;
            
            // Create new command
            new_cmd = new_command();
            if (!new_cmd)
            {
                set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
                goto error;
            }
            
            // Assign tokens to command
            new_cmd->tokens = cmd_start;
            
            // Process tokens to populate command argv and redirections
            if (process_tokens(new_cmd) != SUCCESS)
            {
                set_error(ctx, ERROR_SYNTAX, "Syntax error in command");
                free_commands(&new_cmd);
                goto error;
            }
            
            // Add command to list
            add_command(&commands, new_cmd);
            
            // Set up for next command
            free(current->value); // Free the pipe token value
            cmd_start = current->next;
            free(current); // Free the pipe token
            current = cmd_start;
            prev = NULL;
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
    
    // Add the last command if there is one
    if (cmd_start)
    {
        new_cmd = new_command();
        if (!new_cmd)
        {
            set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
            goto error;
        }
        
        // Assign tokens to command
        new_cmd->tokens = cmd_start;
        
        // Process tokens to populate command argv and redirections
        if (process_tokens(new_cmd) != SUCCESS)
        {
            set_error(ctx, ERROR_SYNTAX, "Syntax error in command");
            free_commands(&new_cmd);
            goto error;
        }
        
        add_command(&commands, new_cmd);
    }
    
    return commands;

error:
    free_commands(&commands);
    free_tokens(&all_tokens);
    return NULL;
}


t_token *tokenize_input(t_general *ctx)
{
    t_token *tokens = NULL;
    t_token *new;
    char *token_value;
    t_token_type token_type;
    int i = 0;
    int len;
    
    while (ctx->input[i])
    {
        // Skip whitespace
        while (ctx->input[i] && is_whitespace(ctx->input[i]))
		i++;
        
        if (!ctx->input[i])
		break;
		
        // Handle quotes
        if (ctx->input[i] == '"' || ctx->input[i] == '\'')
        {
            len = handle_quotes(ctx, i, &token_value);
            if (len < 0)
			goto error;
            new = new_token(token_value, TOKEN_WORD, false);
            if (!new)
            {
                set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
                goto error;
            }
            add_token(&tokens, new);
            i += len;
        }
        // Handle operators
        else if (is_operator(ctx->input[i]))
        {
            len = handle_operator(ctx, i, &token_type, &token_value);
            if (len < 0)
			goto error;
            new = new_token(token_value, token_type, false);
            if (!new)
            {
                set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
                goto error;
            }
            add_token(&tokens, new);
            i += len;
        }
        // Handle $ for variable expansion
        else if (ctx->input[i] == '$')
        {
            len = handle_dollar(ctx, i, &token_value);
            if (len < 0)
			goto error;
            new = new_token(token_value, TOKEN_WORD, true);
            if (!new)
            {
                set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
                goto error;
            }
            add_token(&tokens, new);
            i += len;
        }
        // Handle normal words
        else
        {
            len = handle_word(ctx, i, &token_value);
            if (len < 0)
			goto error;
            new = new_token(token_value, TOKEN_WORD, false);
            if (!new)
            {
                set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
                goto error;
            }
            add_token(&tokens, new);
            i += len;
        }
    }
    
    return tokens;
	
	error:
    free_tokens(&tokens);
    return NULL;
}


void	parse_command(t_general *ctx){

	t_token*	all_tokens = tokenize_input(ctx);
	
	t_token *current;
    char *type_str[] = {"WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "REDIR_APPEND", "HEREDOC"};
    
    current = all_tokens;
    while (current)
    {
        printf("Token: [%s](type: %s))\n", 
               current->value, 
               type_str[current->type]);
        current = current->next;
    }
}