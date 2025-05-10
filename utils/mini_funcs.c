#include "../minishell.h"

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