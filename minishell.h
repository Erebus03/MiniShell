 #include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


// typedef struct s_m
// {
// 	t_cmd *cmd;
// 	char **env;
// 	int exit_status;
// }	t_m;

// typedef struct s_cmd
// {
// 	char **cmd;
// 	t_red *red;
// 	t_cmd *next;
// }	t_cmd;

// typedef struct s_red
// {
// 	char *str;
// 	int type;
// 	t_red *next;
// }	t_red;

typedef struct nde {
	char *cmd;
	struct nde *next;
} node;

static char	**clean(char **cells);
static char	**allocate_and_fill(char **cells, const char *s, int len, int k);
char	**split_core(char **cells, const char *s, char c, int words);
int	wordcount(const char *s, char c);
char	**ft_split(char const *s, char c);
