NAME = philosophers
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -pthread
SRCS = source/main.c \
	   source/ft_atoi.c \
	   source/ft_calloc.c
OBJS = $(SRCS:source/%.c=object/%.o)
OBJDEST = object

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

$(OBJDEST)/%.o: source/%.c
	@mkdir -p $(OBJDEST)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
