/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:57:58 by araji             #+#    #+#             */
/*   Updated: 2025/05/11 14:44:07 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    bool            t_general; // was this token expanded (from variable)
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

void list_env_vars(t_env_var **envlst, char **envp);
void	copy_envp(t_general *ctx);
void	parse_command(t_general *ctx);



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
t_env_var	*new_var(char *key, char *value);
