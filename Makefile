NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror

LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

SRCS		=	src/builtins/builtins_utils.c \
				src/builtins/ft_cd.c \
				src/builtins/ft_echo.c \
				src/builtins/ft_env.c \
				src/builtins/ft_exit.c \
				src/builtins/ft_export.c \
				src/builtins/ft_pwd.c \
				src/builtins/ft_unset.c \
				src/executor/exec_builtin.c \
				src/executor/executor.c \
				src/parser/parse.c \
				src/parser/parse_functions.c \
				src/parser/utils/parse_pipes.c \
				src/parser/utils/parse_quotes.c \
				src/signals/signals.c \
				src/ft_error.c \
				src/minishell.c \
				src/utils/utils_1.c \
				src/utils/redir_utils.c \

OBJS		= $(SRCS:.c=.o)

RM			= rm -f

INCLUDES	= -I includes

all: $(NAME)

$(NAME): $(OBJS)
	@make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	@echo "Minishell compilado con éxito."

%.o: %.c includes/minishell.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

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