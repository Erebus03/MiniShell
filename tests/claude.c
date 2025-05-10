#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>

/**
 * Error codes
 */
typedef enum e_error_code
{
    SUCCESS = 0,
    ERROR_MEMORY,
    ERROR_SYNTAX,
    ERROR_QUOTES,
    ERROR_GENERAL
} t_error_code;

/**
 * Token types according to the 42 minishell subject requirements
 */
typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,       // |
    TOKEN_REDIR_IN,   // <
    TOKEN_REDIR_OUT,  // >
    TOKEN_REDIR_APPEND, // >>
    TOKEN_HEREDOC     // <<
} t_token_type;

/**
 * Token structure with improved organization
 */
typedef struct s_token
{
    char            *value;      // token content
    t_token_type    type;        // token type
    bool            t_general; // was this token expanded (from variable)
    struct s_token  *next;       // next token in the list
} t_token;

/**
 * Redirection structure
 */
typedef struct s_redir
{
    t_token_type    type;        // redirection type
    char            *file;       // file or heredoc delimiter
    int             fd;          // file descriptor (for execution)
    struct s_redir  *next;       // next redirection
} t_redir;

/**
 * Command structure with improved organization
 */
typedef struct s_command
{
    char            **argv;      // command arguments (NULL-terminated)
    int             argc;        // argument count
    t_redir         *redirs;     // redirections
    t_token         *tokens;     // original tokens for this command
    struct s_command *next;      // next command in pipeline
} t_command;

/**
 * Parser context to track state
 */
typedef struct s_parser_ctx
{
    char            *input;      // input string
    char            **envp;      // environment variables
    t_error_code    error;       // last error code
    char            *error_msg;  // detailed error message
    int             exit_status; // last command exit status
} t_general;

/* Forward declarations */
t_token *new_token(char *value, t_token_type type, bool t_general);
void    add_token(t_token **tokens, t_token *new_token);
void    free_tokens(t_token **tokens);
t_command *new_command(void);
void    add_command(t_command **commands, t_command *new_cmd);
void    free_commands(t_command **commands);
t_redir *new_redir(t_token_type type, char *file);
void    add_redir(t_redir **redirs, t_redir *new_redir);
void    free_redirs(t_redir **redirs);
bool    is_whitespace(char c);
bool    is_operator(char c);
int     handle_quotes(t_general *ctx, int i, char **value);
int     handle_word(t_general *ctx, int i, char **value);
int     handle_operator(t_general *ctx, int i, t_token_type *type, char **value);
int     handle_dollar(t_general *ctx, int i, char **value);
t_token *tokenize_input(t_general *ctx);
t_command *parse_commands(t_general *ctx);
t_error_code process_tokens(t_command *cmd);
void    print_tokens(t_token *tokens);
void    print_commands(t_command *commands);
void    set_error(t_general *ctx, t_error_code code, char *msg);
char    *get_env_value(char *var_name, char **envp);

/**
 * Creates a new token
 */
t_token *new_token(char *value, t_token_type type, bool t_general)
{
    t_token *token;
    
    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        return NULL;
    token->value = value;
    token->type = type;
    token->t_general = t_general;
    token->next = NULL;
    return token;
}

/**
 * Adds a token to the end of the token list
 */
void add_token(t_token **tokens, t_token *new_token)
{
    t_token *current;
    
    if (!*tokens)
    {
        *tokens = new_token;
        return;
    }
    current = *tokens;
    while (current->next)
        current = current->next;
    current->next = new_token;
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

/**
 * Creates a new command
 */
t_command *new_command(void)
{
    t_command *cmd;
    
    cmd = (t_command *)malloc(sizeof(t_command));
    if (!cmd)
        return NULL;
    cmd->argv = NULL;
    cmd->argc = 0;
    cmd->redirs = NULL;
    cmd->tokens = NULL;
    cmd->next = NULL;
    return cmd;
}

/**
 * Adds a command to the end of the command list
 */
void add_command(t_command **commands, t_command *new_cmd)
{
    t_command *current;
    
    if (!*commands)
    {
        *commands = new_cmd;
        return;
    }
    current = *commands;
    while (current->next)
        current = current->next;
    current->next = new_cmd;
}

/**
 * Creates a new redirection
 */
t_redir *new_redir(t_token_type type, char *file)
{
    t_redir *redir;
    
    redir = (t_redir *)malloc(sizeof(t_redir));
    if (!redir)
        return NULL;
    redir->type = type;
    redir->file = strdup(file);
    redir->fd = -1;
    redir->next = NULL;
    return redir;
}

/**
 * Adds a redirection to the end of the redirection list
 */
void add_redir(t_redir **redirs, t_redir *new_redir)
{
    t_redir *current;
    
    if (!*redirs)
    {
        *redirs = new_redir;
        return;
    }
    current = *redirs;
    while (current->next)
        current = current->next;
    current->next = new_redir;
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
        if (current->argv)
        {
            for (i = 0; i < current->argc; i++)
                free(current->argv[i]);
            free(current->argv);
        }
        free_redirs(&current->redirs);
        free_tokens(&current->tokens);
        free(current);
        current = next;
    }
    *commands = NULL;
}
void set_error(t_general *ctx, t_error_code code, char *msg)
{
    ctx->error = code;
    if (ctx->error_msg)
        free(ctx->error_msg);
    ctx->error_msg = strdup(msg);
}

bool is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

bool is_operator(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

int handle_quotes(t_general *ctx, int i, char **value)
{
    int start = i;
    char quote_char = ctx->input[i];
    int j = 0;
    char *result;
    
    i++; // Skip opening quote
    while (ctx->input[i] && ctx->input[i] != quote_char)
        i++;
    
    if (!ctx->input[i]) // No closing quote found
    {
        set_error(ctx, ERROR_QUOTES, "Unclosed quote");
        return -1;
    }
    
    // Allocate memory for the result (without quotes)
    result = malloc(i - start);
    if (!result)
    {
        set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
        return -1;
    }
    
    // Copy string without quotes
    j = 0;
    start++; // Skip opening quote
    while (start < i)
        result[j++] = ctx->input[start++];
    result[j] = '\0';
    
    *value = result;
    return i + 2 - start + j; // Return total length processed
}

/**
 * Handles word tokens (non-whitespace, non-operator sequences)
 * Returns the number of characters processed
 */
int handle_word(t_general *ctx, int i, char **value)
{
    int start = i;
    char *result;
    int j = 0;
    
    while (ctx->input[i] && !is_whitespace(ctx->input[i]) && !is_operator(ctx->input[i]))
        i++;
    
    // Allocate memory for the result
    result = malloc(i - start + 1);
    if (!result)
    {
        set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
        return -1;
    }
    
    // Copy word
    j = 0;
    while (start < i)
        result[j++] = ctx->input[start++];
    result[j] = '\0';
    
    *value = result;
    return i - start + j; // Return total length processed
}

/**
 * Handles operator tokens (|, <, >, >>, <<)
 * Returns the number of characters processed
 */
int handle_operator(t_general *ctx, int i, t_token_type *type, char **value)
{
    int len = 1;
    char *result;
    
    // Safely check for double operators (with bound checking)
    bool has_next = (ctx->input[i + 1] != '\0');
    
    // Determine operator type and length
    if (ctx->input[i] == '|')
    {
        *type = TOKEN_PIPE;
    }
    else if (ctx->input[i] == '<')
    {
        if (has_next && ctx->input[i + 1] == '<')
        {
            *type = TOKEN_HEREDOC;
            len = 2;
        }
        else
            *type = TOKEN_REDIR_IN;
    }
    else if (ctx->input[i] == '>')
    {
        if (has_next && ctx->input[i + 1] == '>')
        {
            *type = TOKEN_REDIR_APPEND;
            len = 2;
        }
        else
            *type = TOKEN_REDIR_OUT;
    }
    
    // Allocate memory for the result
    result = malloc(len + 1);
    if (!result)
    {
        set_error(ctx, ERROR_MEMORY, "Memory allocation failed");
        return -1;
    }
    
    // Copy operator
    strncpy(result, ctx->input + i, len);
    result[len] = '\0';
    
    *value = result;
    return len;
}

/**
 * Gets the value of an environment variable
 */
char *get_env_value(char *var_name, char **envp)
{
    int i = 0;
    int var_len = strlen(var_name);
    
    while (envp[i])
    {
        if (strncmp(envp[i], var_name, var_len) == 0 && envp[i][var_len] == '=')
            return strdup(envp[i] + var_len + 1);
        i++;
    }
    return strdup("");
}

/**
 * Handles variable expansion ($VAR)
 * Returns the number of characters processed from input
 */
int handle_dollar(t_general *ctx, int i, char **value)
{
    int start = i;
    char var_name[256] = {0};
    int var_len = 0;
    
    i++; // Skip $
    
    // Special case for $?
    if (ctx->input[i] == '?')
    {
        char exit_str[12]; // More than enough for an integer
        snprintf(exit_str, sizeof(exit_str), "%d", ctx->exit_status);
        *value = strdup(exit_str);
        return 2; // $ + ?
    }
    
    // Extract variable name
    while (ctx->input[i] && (isalnum(ctx->input[i]) || ctx->input[i] == '_'))
    {
        if (var_len < 255)
            var_name[var_len++] = ctx->input[i];
        i++;
    }
    var_name[var_len] = '\0';
    
    // Get variable value
    if (var_len > 0)
        *value = get_env_value(var_name, ctx->envp);
    else
        *value = strdup("$"); // Just $ without variable name
    
    return i - start;
}

/**
 * Tokenizes the input string into tokens
 */
t_token *tokenize_input(t_general *ctx)
{
    t_token *tokens = NULL;
    t_token *new;
    char *token_value;
    t_token_type token_type;
    int i = 0;
    int len;
    bool t_general;
    
    while (ctx->input[i])
    {
        // Skip whitespace
        while (ctx->input[i] && is_whitespace(ctx->input[i]))
            i++;
        
        if (!ctx->input[i])
            break;
        
        t_general = false;
        
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
    
    // Allocate argv array
    cmd->argv = (char **)malloc(sizeof(char *) * (arg_count + 1));
    if (!cmd->argv)
        return ERROR_MEMORY;
    
    // Fill argv array and process redirections
    current = cmd->tokens;
    arg_count = 0;
    
    while (current)
    {
        next = current->next;
        
        if (current->type == TOKEN_WORD)
        {
            cmd->argv[arg_count] = strdup(current->value);
            if (!cmd->argv[arg_count])
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
    cmd->argv[arg_count] = NULL;
    return SUCCESS;
}

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
        
        // Print argv
        printf("  Arguments (%d):", current->argc);
        for (j = 0; j < current->argc; j++)
            printf(" [%s]", current->argv[j]);
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

/**
 * Initialize parser context
 */
void init_general(t_general *ctx, char *input, char **envp, int exit_status)
{
    ctx->input = input;
    ctx->envp = envp;
    ctx->error = SUCCESS;
    ctx->error_msg = NULL;
    ctx->exit_status = exit_status;
}

/**
 * Free parser context resources
 */
void free_parser_ctx(t_general *ctx)
{
    if (ctx->error_msg)
        free(ctx->error_msg);
    ctx->error_msg = NULL;
}

/**
 * Example usage
 */

int main(int argc, char **argv, char **envp)
{
    char input[1024];
    t_command *commands;
    t_general ctx;
    int exit_status = 0;
    
    while (1)
    {
        printf("claudeshell> ");
        if (fgets(input, 1024, stdin) == NULL)		//	readline()
            break;
        
        // Remove trailing newline
        input[strcspn(input, "\n")] = 0;
        
        // Exit if command is "exit"
        if (strcmp(input, "exit") == 0)
            break;
        
        // Initialize parser context
        init_general(&ctx, input, envp, exit_status);
        
        // Parse commands
        commands = parse_commands(&ctx);
        
        // Handle errors
        if (ctx.error != SUCCESS)
        {
            printf("Error: %s\n", ctx.error_msg);
        }
        else if (commands)
        {
            // Print and free commands
            print_commands(commands);
            free_commands(&commands);
        }
        // Free parser context resources
        free_parser_ctx(&ctx);
    }
    return 0;
}