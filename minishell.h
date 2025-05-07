#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_error_code
{
    SUCCESS = 0,
    ERROR_MEMORY,
    ERROR_SYNTAX,
    ERROR_QUOTES,
    ERROR_GENERAL
}	t_error_code;

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,       // |
    TOKEN_REDIR_IN,   // <
    TOKEN_REDIR_OUT,  // >
    TOKEN_REDIR_APPEND, // >>
    TOKEN_HEREDOC     // <<
}	t_token_type;

typedef struct s_token
{
    char            *value;      // token content
    t_token_type    type;        // token type
    bool            is_expanded; // was this token expanded (from variable)
    struct s_token  *next;       // next token in the list
}	t_token;

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}	t_env_var;

typedef struct s_redir
{
    t_token_type    type;        // redirection type
    char            *file;       // file or heredoc delimiter
    int             fd;          // file descriptor (for execution)
    struct s_redir  *next;       // next redirection
}	t_redir;

typedef struct s_command
{
    char            **cmd;      // command arguments (NULL-terminated)
    t_redir         *redirs;     // redirections
    t_token         *tokens;     // original tokens for this command
    struct s_command *next;      // next command in pipeline
}	t_command;

typedef struct s_general
{
    char            *input;		// input string
    t_env_var		*envlst;	// environment variables
    char			**envarr;	// environment variables
    t_error_code    error;		// last error code
    char            *error_msg;	// detailed error message
    int             exit_status;// last command exit status
}	t_general;

t_env_var* list_env_vars(t_env_var **envlst, char **envp);