NAME = pipex

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
OBJ_DIR = obj
LIBFT = LIBFT/libft.a

SRCS = \
	find_path.c \
	parse_argv.c \
	main.c \


OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L./LIBFT -lft

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $(notdir $<)..."
	@$(CC) $(CFLAGS) -I. -I LIBFT/ -o $@ -c $<

$(LIBFT):
	@echo "Building libft..."
	@$(MAKE) -C LIBFT

clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "Cleaning libraries and executable..."
	@$(MAKE) -C LIBFT fclean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

