#include "../minishell.h"

/**
 * Process tokens to populate command argv and redirections
 */
t_error_code process_tokens(t_command *cmd)
{
    t_token *current = cmd->tokens;
    t_token *next;
    int arg_count = 0;
    t_redir *redir;
    
    // First count arguments
    while (current)
    {
        if (current->type == TOKEN_WORD)
            arg_count++;
        else if (current->type != TOKEN_PIPE)
        {
            // Redirections need a file name
            if (!current->next || current->next->type != TOKEN_WORD)
                return ERROR_SYNTAX;
            current = current->next; // Skip the file name
        }
        current = current->next;
    }
    
    // Allocate cmd array
    cmd->cmd = (char **)malloc(sizeof(char *) * (arg_count + 1));
    if (!cmd->cmd)
        return ERROR_MEMORY;
    
    // Fill argv array and process redirections
    current = cmd->tokens;
    arg_count = 0;
    
    while (current)
    {
        next = current->next;
        
        if (current->type == TOKEN_WORD)
        {
            cmd->cmd[arg_count] = strdup(current->value);
            if (!cmd->cmd[arg_count])
                return ERROR_MEMORY;
            arg_count++;
        }
        else if (current->type != TOKEN_PIPE)
        {
            // Create redirection
            if (next && next->type == TOKEN_WORD)
            {
                redir = new_redir(current->type, next->value);
                if (!redir)
                    return ERROR_MEMORY;
                add_redir(&cmd->redirs, redir);
                current = next; // Skip the file name
                next = current->next;
            }
        }
        current = next;
    }
    cmd->cmd[arg_count] = NULL;
    return SUCCESS;
}
