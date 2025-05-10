#include "../minishell.h"




/**
 * Print all tokens in a command for debugging
 */
void print_tokens(t_token *tokens)
{
    t_token *current;
    char *type_str[] = {"WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "REDIR_APPEND", "HEREDOC"};
    
    current = tokens;
    while (current)
    {
        printf("Token: [%s] (type: %s, expanded: %s)\n", 
               current->value, 
               type_str[current->type], 
               current->t_general ? "yes" : "no");
        current = current->next;
    }
}

/**
 * Print a command's redirections for debugging
 */
void print_redirections(t_redir *redirs)
{
    t_redir *current;
    char *type_str[] = {"WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "REDIR_APPEND", "HEREDOC"};
    
    current = redirs;
    while (current)
    {
        printf("  Redirection: type=%s, file=%s\n", 
               type_str[current->type], 
               current->file);
        current = current->next;
    }
}

/**
 * Print all commands for debugging
 */
void print_commands(t_command *commands)
{
    int i = 1;
    t_command *current = commands;
    int j;
    
    while (current)
    {
        printf("Command %d:\n", i++);
        

        for (j = 0; current->cmd != NULL; j++)
            printf(" [%s]", current->cmd[j]);
        printf("\n");
        
        // Print redirections
        if (current->redirs)
        {
            printf("  Redirections:\n");
            print_redirections(current->redirs);
        }
        
        // Print original tokens
        printf("  Original Tokens:\n");
        print_tokens(current->tokens);
        
        printf("\n");
        current = current->next;
    }
}

