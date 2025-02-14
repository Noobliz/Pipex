NAME = pipex
NAME_BONUS = pipex_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
OBJ_DIR = obj
OBJ_DIR_BONUS = obj_bonus
LIBFT = LIBFT/libft.a

# Fichiers de la partie obligatoire
SRCS = \
	find_path.c \
	free_error_utils.c \
	lst_utils.c \
	children_utils.c \
	children.c \
	main.c

# Fichiers pour le bonus (dans le dossier bonus/)
SRCS_BONUS = \
	bonus/find_path.c \
	bonus/free_error_utils.c \
	bonus/lst_utils.c \
	bonus/children_utils.c \
	bonus/children.c \
	bonus/main_bonus.c  # Fichier différent pour la gestion du bonus

OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
OBJS_BONUS = $(SRCS_BONUS:bonus/%.c=$(OBJ_DIR_BONUS)/%.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@echo "Compiling Pipex..."
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L./LIBFT -lft
	@echo "Pipex compilation complete ✅"

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(LIBFT) $(OBJS_BONUS)
	@echo "Compiling Pipex Bonus..."
	@$(CC) $(CFLAGS) $(OBJS_BONUS) -o $(NAME_BONUS) -L./LIBFT -lft
	@echo "Pipex Bonus compilation complete ✅"

# Compilation des fichiers dans obj/
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -I. -I LIBFT/ -o $@ -c $@

# Compilation des fichiers bonus dans obj_bonus/
$(OBJ_DIR_BONUS)/%.o: bonus/%.c
	@mkdir -p $(OBJ_DIR_BONUS)
	@$(CC) $(CFLAGS) -I. -I LIBFT/ -o $@ -c $@

$(LIBFT):
	@echo "Building libft..."
	@$(MAKE) -C LIBFT

clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJ_DIR) $(OBJ_DIR_BONUS)

fclean: clean
	@echo "Cleaning libraries and executables..."
	@$(MAKE) -C LIBFT fclean
	@rm -f $(NAME) $(NAME_BONUS)

re: fclean all

.PHONY: all bonus clean fclean re
