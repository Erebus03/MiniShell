#include "../minishell.h"

void free_commands(t_command **commands)
{
    t_command *current;
    t_command *next;
    int i;
    if (!commands || !*commands)
        return;
    current = *commands;
    while (current)
    {
        next = current->next;
        if (current->cmd)
        {
            for (i = 0; current->cmd != NULL; i++)
                free(current->cmd[i]);
            free(current->cmd);
        }
        free_redirs(&current->redirs);
        free_tokens(&current->tokens);
        free(current);
        current = next;
    }
	write(1, "freed commands\n", 15);
    *commands = NULL;
}

/**
 * Frees all redirections in the list
 */
void free_redirs(t_redir **redirs)
{
    t_redir *current;
    t_redir *next;
    
    if (!redirs || !*redirs)
        return;
    current = *redirs;
    while (current)
    {
        next = current->next;
        free(current->file);
        free(current);
        current = next;
    }
    *redirs = NULL;
}

/**
 * Frees all tokens in the list
 */
void free_tokens(t_token **tokens)
{
    t_token *current;
    t_token *next;
    
    if (!tokens || !*tokens)
        return;
    current = *tokens;
    while (current)
    {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
    *tokens = NULL;
}

