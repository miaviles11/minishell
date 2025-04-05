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
				src/parser/create_command_node/process_arguments_1.c \
				src/parser/create_command_node/process_arguments_2.c \
				src/parser/create_command_node/process_arguments_utils.c \
				src/parser/perform_expansion/expansion_utils_1.c \
				src/parser/perform_expansion/expansion_utils_2.c \
				src/parser/perform_expansion/expansion_utils_3.c \
				src/parser/perform_expansion/expansion_utils_4.c \
				src/parser/validate_input/p_pipes_utils.c \
				src/parser/validate_input/parse_pipes.c \
				src/parser/validate_input/parse_quotes.c \
				src/parser/parser.c \
				src/parser/parser_functions.c \
				src/signals/signals.c \
				src/ft_error.c \
				src/initialize_minishell.c \
				src/minishell.c \
				utils/utils_1.c \
				utils/redir_utils.c \

OBJS		= $(SRCS:.c=.o)

RM			= rm -f

INCLUDES	= -I includes

all: $(NAME)

$(NAME): $(OBJS)
	@make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
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