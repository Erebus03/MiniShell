/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <araji@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:57:58 by araji             #+#    #+#             */
/*   Updated: 2025/06/30 11:30:39 by araji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

/* ================================ INCLUDES ================================ */
#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

/* ================================ ENUMS =================================== */
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
    TWORD,
    TPIPE,			// |
    TREDIR_IN,		// <
    TREDIR_OUT,		// >
    TREDIR_APPEND,	// >>
    THEREDOC		// <<
}	t_token_type;

/* ============================== STRUCTURES ============================== */
typedef struct s_token
{
    char            *value;
    t_token_type    type;
    bool            expanded;
    int				quoted_delim;
    struct s_token  *next;
    struct s_token  *prev;
}	t_token;

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}	t_env_var;

typedef struct s_redir
{
    t_token_type    type;
    char            *file;
    int             fd;
	int				expand_in_heredec;
    struct s_redir  *next;
}	t_redir;

typedef struct s_command
{
    char            **cmd;
    t_redir         *redirs;
    t_token         *tokens;
    struct s_command *next;
}	t_command;

typedef struct s_memory
{
	void			*ptr;
	struct s_memory	*next;
}	t_memory;

typedef struct s_general
{
    char			*input;
    t_env_var		*envlst;
    char			**envarr;
    int				error;
	t_memory		*heap;
    char			*error_msg;
    int				exit_status;
    int				no_expand_heredoc;
    int				inside_env_var;
}	t_general;

/* ========================= CONSTRUCTOR FUNCTIONS ========================= */
t_token			*new_token(t_general *ctx, char *value, t_token_type type, bool expanded);
t_command		*new_command(t_general *ctx);
t_env_var		*new_var(t_general *ctx, char *key, char *value);
t_redir			*new_redir(t_general *ctx, t_token_type type, char *file);

/* =========================== LIST FUNCTIONS ============================= */
void			add_token(t_token **tokens, t_token *new_token);
void			add_redir(t_redir **redirs, t_redir *new_redir);
void			add_command(t_command **commands, t_command *new_cmd);
void			join_tokens(t_token *tokens, t_token *new);
int				tokens_size(t_token *lst);
t_token			*last_token(t_token *lst);

/* ========================== CLEANUP FUNCTIONS ============================ */
void			free_tokens(t_token **tokens);
void			free_redirs(t_redir **redirs);
void			free_commands(t_command **commands);
void			free_envp(t_general *ctx, int mode);
void			cleanup(t_general *ctx);
void			clean_exit(t_general *ctx, char *errmsg, int errcode);

/* ======================== ENVIRONMENT FUNCTIONS ========================== */
void			list_env_vars(t_env_var **envlst, char **envp);
void			copy_envp(t_general *ctx);
char			*get_env_value(char *var_name, t_env_var *lst);

/* ========================== PARSING FUNCTIONS ============================ */
t_command		*parse_command(t_general *ctx);
t_token			*tokenize_input(t_general *ctx);
int				process_tokens(t_general *ctx, t_command *cmd);
int				validate_quotes(t_general *ctx);
int				check_syntax(t_general *ctx, t_token* tokens);

/* ========================= TOKENIZING FUNCTIONS ========================== */
int				handle_quotes(t_general *ctx, int i, char **value);
int				handle_word(t_general *ctx, int i, char **value);
int				handle_operator(t_general *ctx, int i, t_token_type *type, char **value);
int				handle_dollar(t_general *ctx, int i, char **value);
int				process_single_token(t_general *ctx, int i, void **tkn_ptrs, int *skipped);
int				process_quoted_token(t_general *ctx, int i, t_token **tokens, t_token **last_added);
int				process_operator_token(t_general *ctx, int i, t_token **tokens, t_token **last_added);
int				process_dollar_token(t_general *ctx, int i, void **tkn_ptrs, int *skipped);
int				process_word_token(t_general *ctx, int i, t_token **tokens, t_token **last_added);
void			handle_token_joining(t_token *tokens, t_token *new, int skipped);
int				skip_whitespace_and_track(t_general *ctx, int *i, int *skipped);

/* ========================= EXPANSION FUNCTIONS =========================== */
int				calculate_expansion_size(t_general *ctx, int start, char stop_char);
int				build_exp_str(t_general *ctx, int start, char stop_char, char *result);
int				to_be_split(char *value);
t_token			*split_token_value(char *value);

/* ========================== UTILITY FUNCTIONS ============================ */
bool			is_whitespace(char c);
bool			is_operator(char c);
bool			is_quote(char c);
// void			set_error(t_general *ctx, int code, char *msg);
void			init_general_struct(t_general *context, char *value);
int				ft_strcmp(const char *s1, const char *s2);
char			*ft_strcpy(char *dest, const char *src);
char			*ft_strncpy(char *dest, const char *src, size_t n);
void			increment_val(int *value1, int *value2, int *value3, int *value4);

/* ========================== MEMORY FUNCTIONS ============================== */
void			*allocate(t_general *ctx, size_t size);
t_memory		*new_addr(void *ptr);
void			add_addr(t_general *ctx, t_memory *new_addr);

/* ========================== DEBUG FUNCTIONS ============================== */
void			print_tokens(t_token *tokens);
void			print_commands(t_command *commands);

#endif

/*
#ifndef MINISHELL_H
#define MINISHELL_H

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
    TWORD,
    TPIPE,			// |
    TREDIR_IN,		// <
    TREDIR_OUT,		// >
    TREDIR_APPEND,	// >>
    THEREDOC		// <<
}	t_token_type;

typedef struct s_token
{
    char            *value;				// token content
    t_token_type    type;				// token type
    bool            expanded;			// was this token expanded (from variable)
    struct s_token  *next;				// next token in the list
    struct s_token  *prev;				// prev token
    int				quoted_delim;	// last command exit status
}	t_token;

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}	t_env_var;

typedef struct s_redir
{
    t_token_type    type;        		// redirection type
    char            *file;       		// file or heredoc delimiter
    int             fd;          		// file descriptor (for execution)
	int				expand_in_heredec;	// should i expand inside the heredec or not
    struct s_redir  *next;       		// next redirection
}	t_redir;

typedef struct s_command
{
    char            **cmd;		// command arguments (NULL-terminated)
    t_redir         *redirs;	// redirections
    t_token         *tokens;	// original tokens for this command
    struct s_command *next;		// next command in pipeline
}	t_command;

typedef struct s_memory
{
	void			*ptr;
	struct s_memory	*next;
}	t_memory;

typedef struct s_general
{
    char			*input;				// input string
    t_env_var		*envlst;			// environment variables
    char			**envarr;			// environment variables
    int				error;				// last error code
	t_memory		*heap;
    char			*error_msg;			// detailed error message
    int				exit_status;		// last command exit status
    int				no_expand_heredoc;	// last command exit status
    int				inside_env_var;		// should treat redir inside env vars
}	t_general;

t_token			*new_token(char *value, t_token_type type, bool expanded);
t_command		*new_command(void);
t_env_var		*new_var(char *key, char *value);
t_redir			*new_redir(t_token_type type, char *file);
void			add_token(t_token **tokens, t_token *new_token);
void			add_redir(t_redir **redirs, t_redir *new_redir);
void			add_command(t_command **commands, t_command *new_cmd);
void			free_tokens(t_token **tokens);
void			free_redirs(t_redir **redirs);
void			free_commands(t_command **commands);
void			free_envp(t_general *ctx, int mode);

void			list_env_vars(t_env_var **envlst, char **envp);
void			copy_envp(t_general *ctx);
char			*get_env_value(char *var_name, t_env_var *lst);
t_command		*parse_command(t_general *ctx);
t_token			*tokenize_input(t_general *ctx);
int				process_tokens(t_command *cmd);
int				handle_quotes(t_general *ctx, int i, char **value);
int				handle_word(t_general *ctx, int i, char **value);
int				handle_operator(t_general *ctx, int i, t_token_type *type, char **value);
int				handle_dollar(t_general *ctx, int i, char **value);
void			print_tokens(t_token *tokens);
void			print_commands(t_command *commands);
bool			is_whitespace(char c);
bool			is_operator(char c);
bool			is_quote(char c);
void			set_error(t_general *ctx, int code, char *msg);


char			*ft_strcpy(char *dest, const char *src);
char			*ft_strncpy(char *dest, const char *src, size_t n);
void			increment_val(int *value1, int *value2, int *value3, int *value4);

void			join_tokens(t_token *tokens, t_token *new);
int				tokens_size(t_token *lst);
t_token			*last_token(t_token *lst);

void			init_general_struct(t_general *context, char *value);
int				validate_quotes(t_general *ctx);

int				calculate_expansion_size(t_general *ctx, int start, char stop_char);
int				build_exp_str(t_general *ctx, int start, char stop_char, char *result);

int				to_be_split(char *value);
t_token			*split_token_value(char *value);
int				ft_strcmp(const char *s1, const char *s2);


void			cleanup(t_general *ctx);
int				check_syntax(t_general *ctx, t_token* tokens);


void			clean_exit(t_general *ctx, char *errmsg, int errcode);


int				process_single_token(t_general *ctx, int i, t_token **tokens, int *skipped, t_token **last_added);
int				process_quoted_token(t_general *ctx, int i, t_token **tokens, t_token **last_added);
int				process_operator_token(t_general *ctx, int i, t_token **tokens, t_token **last_added);
int				process_dollar_token(t_general *ctx, int i, t_token **tokens, int *skipped, t_token **last_added);
int				process_word_token(t_general *ctx, int i, t_token **tokens, t_token **last_added);
void			handle_token_joining(t_token *tokens, t_token *new, int skipped);
int				skip_whitespace_and_track(t_general *ctx, int *i, int *skipped);

#endif
*/