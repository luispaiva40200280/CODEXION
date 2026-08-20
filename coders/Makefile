NAME = codexion
CC = cc -Wall -Wextra -Werror -pthread -I
SRCS = main.c init/checker.c init/init.c init/parser.c init/queue.c init/queue_utils.c \
    simulation/coder.c simulation/ft_time.c simulation/monitor.c simulation/routine.c

all: $(NAME)

$(NAME):
	@echo "\033[32m Compiling Codexion...\033[0m "
	@$(CC) includes $(SRCS) -g -o $(NAME)

re: 
	@echo "\033[32m Recompiling Codexion...\033[0m "
	@$(CC) includes $(SRCS) -g -o $(NAME)

clean:
	@echo "\033[38;5;203m Removing Codexion...\033[0m "
	@rm -rf $(NAME)

fclean: $(clean)

lint:
	norminette

.PHONY: all clean fclean re $(NAME)
