NAME = shell
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = main.c utils/env_vars.c utils/parse_cmd.c utils/add_nodes.c utils/free_nodes.c utils/handler.c \
	utils/mini_funcs.c utils/mini_funcs2.c utils/new_nodes.c utils/process_tokens.c utils/tokenizer.c \
	utils/expansion.c utils/check_syntax.c utils/mem_manager.c utils/print_stuff.c
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