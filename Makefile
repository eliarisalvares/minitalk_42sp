RED = \033[31m
GREEN = \033[32m
BLUE = \033[34m
WHITE = \033[39m
RESET = \033[0m

CC = clang

CFLAGS = -Wall -Wextra -Werror -g

INC = -Iincludes/ -Ilibft/ \

LIBS = -Llibft -lft

SERVER = server
SRCS_SERVER = ./srcs/server/server.c
OBJS_SERVER = $(SRCS_SERVER:.c=.o)

CLIENT = client
SRCS_CLIENT = ./srcs/client/client.c
OBJS_CLIENT = $(SRCS_CLIENT:.c=.o)

all: $(CLIENT) $(SERVER)
	@echo "$(GREEN)Minitalk ready!"

.c.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@


$(CLIENT): $(OBJS_CLIENT)
	@echo "Compiling libft ..."
	@$(MAKE) -sC libft
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "$(BLUE)Client ready.$(RESET)"

$(SERVER): $(OBJS_SERVER)
	@echo "Minitalk creating libft ..."
	@$(MAKE) -sC libft
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "$(BLUE)Server ready.$(RESET)"

clean:
	@$(MAKE) -sC libft clean
	@rm -f $(OBJS_CLIENT) $(OBJS_SERVER)

fclean: 
	@$(MAKE) -sC libft fclean
	@rm -f $(OBJS_CLIENT) $(OBJS_SERVER)
	@rm -f $(CLIENT) $(SERVER)

re: fclean all

.PHONY: all clean fclean re