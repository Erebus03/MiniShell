NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror  -fsanitize=address -g3

SRC = main.c utils/env_vars.c utils/parse_cmd.c utils/add_nodes.c utils/free_nodes.c utils/handler.c \
		utils/mini_funcs.c utils/mini_funcs2.c utils/mini_funcs3.c utils/new_nodes.c utils/process_tokens.c \
		utils/tokenizer.c utils/tokenizer2.c utils/expansion.c utils/check_syntax.c utils/mem_manager.c utils/print_stuff.c \
		exucution/eroor_msg.c  exucution/genere_token.c exucution/herdooc.c  exucution/bultin_cd.c exucution/bultin_echo_pwd.c \
		exucution/bultin_env_unset.c  exucution/bultin_exit.c  exucution/bultin.c exucution/butlin_export1.c exucution/butlin_export2.c \
		exucution/butlin_export3.c exucution/exucute_cmd.c exucution/fonctionn.c  exucution/genere_pipe.c exucution/signale.c  exucution/token.c 

OBJ = $(SRC:.c=.o)

LIBFT = libft/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C libft

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C libft clean
	rm -f $(OBJ)

fclean: clean
	$(MAKE) -C libft fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re