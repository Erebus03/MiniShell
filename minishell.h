/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamiri <alamiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:57:58 by araji             #+#    #+#             */
/*   Updated: 2025/07/02 21:03:14 by alamiri          ###   ########.fr       */
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include<unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>
#define MAX_TOKENS 1024
#include <sys/stat.h>
#include <signal.h>
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
     char            *index; 
    char            *file;
    int             fd;
	int				expand_in_heredec; //
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
    t_command        *cmnd ;
    char			*error_msg;
    int				exit_status;
    int				no_expand_heredoc;
    int				inside_env_var;
    char *pwd;
}	t_general;

extern t_general generale;

/* ========================= CONSTRUCTOR FUNCTIONS ========================= */
t_token			*new_token(t_general *ctx, char *value, t_token_type type, bool expanded);
t_command		*new_command(t_general *ctx);
t_env_var		*new_var(t_general *ctx, char *key, char *value);
t_redir			*new_redir(t_general *ctx, t_token_type type, char *file);

/* =========================== LIST FUNCTIONS ============================= */
void			add_token(t_token **tokens, t_token *new_token);
void			add_redir(t_redir **redirs, t_redir *new_redir);
void			add_command(t_command **commands, t_command *new_cmd);
void			add_variable(t_env_var **lst, t_env_var *new_var);
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
void			list_env_vars(t_general *ctx, t_env_var **envlst, char **envp);
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
int				add_addr(t_general *ctx, t_memory *new_addr);

/* ========================== DEBUG FUNCTIONS ============================== */
void			print_tokens(t_token *tokens);
void			print_commands(t_command *commands);



size_t ftt_strlen(const char **s);
void eroor_msg(char * str, int flag);
void eroor_export(char *str);
void eroor_exit(char *str);
void eroor_cd(char *str);
int tokencomnd(t_redir *cout, t_general *data);
int handel_tokeredir(t_redir *cout,t_general *data,int flag);
int tokeredir(t_redir *cout,t_general *data);
int tokeappend(t_redir *cout,t_general *data);
int size_list(t_command *var);
void heredoc_sigint_handler();
char *namefile();
void child_herdoc(t_redir *var);
int parent_herdoc(int pid,t_redir *var,char * name);
int herdocc(t_redir *var, int index);
int token_out(t_redir *temp);
char *getenvp(char **p);
char **reserve_space(t_command *var);
void exucutecmd(char **env, char *path,t_command *var);
char *cherche_path(t_env_var **env);
int  chek_eroorsplit(t_general *data);
void eroor_exucutecmn(char *strjoncmd);
void split_pathexucutecmd(char *path,t_general *data);
void split_chek(t_general *data);
int ft_herdoc(t_command *commands);
int ft_tokin_redir(t_redir *com,t_general *data);
int ft_redir_append(t_redir *com,t_general *data);
int ft_redir_in(t_redir *com,t_general *data);
int  ft_redir_herdoc(t_redir *com);
int  chek_type(t_redir * cc,t_general * data);
void	handle_child_process(t_general *data, int *fd_sa, int *fd);
 void	handle_parent_process(int *fd_sa, int *fd, t_general *data);
void	wait_all_processes(int *pids, int count);
void	exit_error(char *msg);
int	*alloc_pids(t_general *data);
void	ft_exucutepipe(t_general *data);
int  chek_bultin(t_command * var);
void edit_env(t_env_var *var,char *newpath);
void  handel_cdhome(t_general *data);
void execute_cd(t_general *data);
// void	*ft_memcpy(void *dest, const void *src, size_t n);
int chek_newline(char * var);
void process_echo(t_general *data,int i);
void execute_echo(t_general *data);
char *getpath(t_env_var **pp);
void execute_pwd(t_general *data);
// int	ft_isalpha(int c);
int chek_number(char *s);
void singel_exit();
void three_exit();
void exit_num_normal(int n);
void exit_num(int n);
void execute_exit(t_command * var);
void execute_env(t_env_var **env);
int chek_export(char *var);
int  chek_exp(t_env_var **var,char *key);
void afficher_exp(t_env_var *env);
char *extract_key(char *cmd_arg, int j);
int variable_exists(t_env_var *env_list, char *key);
char *extract_value(char *cmd_arg, int start_pos);
void update_existing_variable(t_env_var *env_list, char *key, char *value);
int handle_value_var(t_general *data, char *key, char *value);
int handle_no_value_export(t_general *data, char *key);
 int handle_with_value_export(t_general *data, char *arg, int j);
 int process_single_export_arg(t_general *data, char *arg);
 void process_all_export_args(t_general *data);
void execute_export(t_general *data);
void	free_env_var(t_env_var *env_var);
void	remove_env_var(t_general *data, char *var_name);
void	execute_unset(t_general *data);
void aplementation_bultin(t_general *data);
void sighandler();
void ft_control();








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