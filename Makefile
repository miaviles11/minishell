NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror

LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

SRCS		=	src/main.c \

OBJS		= $(SRCS:.c=.o)

RM			= rm -f

all: $(NAME)

$(NAME): $(OBJS)
	@make -C $(LIBFT_DIR)          # Compilamos primero la libft
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	@echo "Minishell compilado con éxito."

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	@make clean -C $(LIBFT_DIR)
	@echo "Archivos objeto eliminados."

fclean: clean
	$(RM) $(NAME)
	@make fclean -C $(LIBFT_DIR)
	@echo "Binario y librería eliminados."

re: fclean all

.PHONY: all clean fclean re
