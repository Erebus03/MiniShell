/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji <rajianwar421@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:57:58 by araji             #+#    #+#             */
/*   Updated: 2025/06/24 22:50:13 by araji            ###   ########.fr       */
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
    char            *input;				// input string
    t_env_var		*envlst;			// environment variables
    char			**envarr;			// environment variables
    t_error_code    error;				// last error code
	t_memory		*heap;
    char            *error_msg;			// detailed error message
    int             exit_status;		// last command exit status
    int             no_expand_heredoc;	// last command exit status
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
t_error_code	process_tokens(t_command *cmd);
int				handle_quotes(t_general *ctx, int i, char **value);
int				handle_word(t_general *ctx, int i, char **value);
int				handle_operator(t_general *ctx, int i, t_token_type *type, char **value);
int				handle_dollar(t_general *ctx, int i, char **value);
void			print_tokens(t_token *tokens);
void			print_commands(t_command *commands);
bool			is_whitespace(char c);
bool			is_operator(char c);
bool			is_quote(char c);
void			set_error(t_general *ctx, t_error_code code, char *msg);


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