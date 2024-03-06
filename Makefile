NAME = philosophers
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -pthread
SRCS = source/main.c \
       source/ft_atoi.c \
       source/ft_calloc.c \
	   source/init.c \
	   source/routine.c \
	   source/utils.c \
	   source/eating.c \
	   source/dead_check.c

OBJS = $(SRCS:source/%.c=object/%.o)
OBJDEST = object

GREEN = \033[38;2;4;231;98m
BLUE = \033[38;2;119;166;182m
RED = \033[0;31m

RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Linking...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)Compilation successful!$(RESET)"

$(OBJDEST)/%.o: source/%.c
	@mkdir -p $(OBJDEST)
	@echo "$(BLUE)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)Compiled $< successfully!$(RESET)"

clean:
	@echo "$(RED)Cleaning up object files...$(RESET)"
	@rm -f $(OBJS)
	@echo "$(RED)Cleaned.$(RESET)"

fclean: clean
	@echo "$(RED)Removing executable $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(RED)Removed.$(RESET)"

re: fclean all

.PHONY: all clean fclean re
