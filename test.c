#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct s_token
{
    char *value;
    int type;
    struct s_token *next;
} t_token;

// Token types
enum e_token_type
{
    TOKEN_WORD = 0,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_HEREDOC
};

/**
 * Allocates memory and returns a new token
 */
t_token *new_token(char *value, int type)
{
    t_token *token;
    
    token = malloc(sizeof(t_token));
    if (!token)
        return NULL;
    token->value = value;
    token->type = type;
    token->next = NULL;
    return token;
}

/**
 * Adds a token to the end of the token list
 */
void add_token(t_token **tokens, t_token *new)
{
    t_token *current;
    
    if (!*tokens)
    {
        *tokens = new;
        return;
    }
    current = *tokens;
    while (current->next)
        current = current->next;
    current->next = new;
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
 * Returns true if the character is a token separator
 */
int is_separator(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '|' || c == '<' || c == '>');
}

/**
 * Extracts a quoted string from input
 * Returns the length of the extracted string (including quotes)
 */
int extract_quoted_string(char *input, char **result, char quote_char)
{
    int i = 1; // Skip the opening quote
    int len = 0;
    int has_closing_quote = 0;
    
    // Find the closing quote
    while (input[i] && input[i] != quote_char) //skip but regarde $ for expantion
        i++;

    if (input[i] == quote_char)
    {
        has_closing_quote = 1;
        i++; // Include the closing quote
    }
    
    // Allocate memory for the result (without quotes)
    len = i - (has_closing_quote ? 2 : 1);
    *result = malloc(len + 1);
    if (!*result)
        return 0;
    
    // Copy the string without the quotes
    strncpy(*result, input + 1, len);
    (*result)[len] = '\0';
    
    return i; // Return the total length (including quotes)
}

/**
 * Extracts a word from input
 * Returns the length of the extracted word
 */
int extract_word(char *input, char **result)
{
    int i = 0;
    
    // Find the end of the word
    while (input[i] && !is_separator(input[i]))
        i++;
    
    // Allocate memory for the result
    *result = malloc(i + 1);
    if (!*result)
        return 0;
    
    // Copy the word
    strncpy(*result, input, i);
    (*result)[i] = '\0';
    
    return i;
}

/**
 * Extracts a special token (pipe or redirection) from input
 * Returns the length of the extracted token and sets the token type
 */
int extract_special_token(char *input, char **result, int *type)
{
    int len = 1;
    
    if (input[0] == '|')
    {
        *type = TOKEN_PIPE;
    }
    else if (input[0] == '<' && input[1] == '<')
    {
        *type = TOKEN_HEREDOC;
        len = 2;
    }
    else if (input[0] == '>')
    {
        if (input[1] == '>')
        {
            *type = TOKEN_REDIR_APPEND;
            len = 2;
        }
        else
            *type = TOKEN_REDIR_OUT;
    }
    else if (input[0] == '<')
    {
        *type = TOKEN_REDIR_IN;
    }
    
    // Allocate memory for the result
    *result = malloc(len + 1);
    if (!*result)
        return 0;
    
    // Copy the token
    strncpy(*result, input, len);
    (*result)[len] = '\0';
    
    return len;
}

t_token *tokenize_command(char *input)
{
    t_token *tokens = NULL;
    t_token *new;
    char *token_value;
    int token_len;
    int type;
    int i = 0;
    
    while (input[i])
    {
        // Skip whitespace
        while (input[i] && isspace(input[i]))
            i++;
        
        if (!input[i])
            break;
        
        // Handle quotes
        if (input[i] == '"' || input[i] == '\'')
        {
            char quote_char = input[i];
            token_len = extract_quoted_string(input + i, &token_value, quote_char);
            if (token_len == 0)
                goto error;
            new = new_token(token_value, TOKEN_WORD);
            if (!new)
                goto error;
            add_token(&tokens, new);
            i += token_len;
        }
        // Handle pipe and redirections
        else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
    	{
            token_len = extract_special_token(input + i, &token_value, &type);
            if (token_len == 0)
                goto error;
            new = new_token(token_value, type);
            if (!new)
                goto error;
            add_token(&tokens, new);
            i += token_len;
        }
        // Handle normal words
        else
        {
            token_len = extract_word(input + i, &token_value);
            if (token_len == 0)
                goto error;
            new = new_token(token_value, TOKEN_WORD);
            if (!new)
                goto error;
            add_token(&tokens, new);
            i += token_len;
        }
    }
    
    return tokens;

error:
    free_tokens(&tokens);
    return NULL;
}

/**
 * Split the input command into commands separated by pipes
 * Each command is a linked list of tokens
 */
t_token **split_commands(char *input)
{
    t_token *tokens;
    t_token *current;
    t_token *next;
    t_token **commands;
    int cmd_count = 1;
    int i = 0;
    tokens = tokenize_command(input);
    if (!tokens)
        return NULL;
    current = tokens;
    while (current)
    {
        if (current->type == TOKEN_PIPE)
            cmd_count++;
        current = current->next;
    }
    commands = malloc(sizeof(t_token *) * (cmd_count + 1));
    if (!commands)
    {
        free_tokens(&tokens);
        return NULL;
    }
    current = tokens;
    commands[0] = current;
    i = 1;
    while (current)
    {
        if (current->type == TOKEN_PIPE)
        {
            next = current->next;
            current->next = NULL;
            if (next)
                commands[i++] = next;
            current = next;
        }
        else
            current = current->next;
    }
    commands[cmd_count] = NULL;
    return commands;
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
        printf("Token: [%s] (type: %s)\n", current->value, type_str[current->type]);
        current = current->next;
    }
}

/**
 * Print all commands for debugging
 */
void print_commands(t_token **commands)
{
    int i = 0;
    
    while (commands[i])
    {
        printf("Command %d:\n", i + 1);
        print_tokens(commands[i]);
        printf("\n");
        i++;
    }
}

/**
 * Free all commands and their tokens
 */
void free_commands(t_token ***commands)
{
    int i = 0;
    
    if (!commands || !*commands)
        return;
    
    while ((*commands)[i])
    {
        free_tokens(&(*commands)[i]);
        i++;
    }
    
    free(*commands);
    *commands = NULL;
}

// Example usage
int main() {
    char input[1024];
    t_token **commands;
    
    printf("minishell> ");
    fgets(input, 1024, stdin);
    
    commands = split_commands(input);
    if (commands)
    {
        print_commands(commands);
        free_commands(&commands);
    }
    
    return 0;
}