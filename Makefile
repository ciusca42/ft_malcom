NAME = ft_malcom
BIN_NAME = mitm

SRC_DIR = src
OBJ_DIR = obj

SRC = $(shell find $(SRC_DIR) -name '*.c')
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

COMPILER = gcc
COMPILE_FLAGS = -Wall -Wextra -Werror -g

INSTALL_DIR = $(HOME)/.local/bin

all: $(NAME)

$(NAME): $(OBJ)
	$(COMPILER) $(COMPILE_FLAGS) $(OBJ) $(LIBFT) -o $(NAME)
	@mkdir -p $(INSTALL_DIR)
	@cp $(NAME) $(INSTALL_DIR)/$(BIN_NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(COMPILER) $(COMPILE_FLAGS) -c $< -o $@

run: all
	./$(NAME)

help:
	@echo "test"

clean:
	@rm -rf $(OBJ_DIR)
	@echo $(GREEN) "CLEAN COMPLETED" $(NONE)

fclean: clean
	@rm -f $(NAME)
	@rm -f $(INSTALL_DIR)/$(BIN_NAME)
	@echo $(GREEN) "FCLEAN COMPLETED" $(NONE)

re: fclean all

.DEFAULT:
	@echo "\nNo rule called: '\033[1;31m$@\033[0m'\n \
	Type: \"\033[1;32mmake help\033[0m\" for more informations\n"

.PHONY: all run help clean fclean re

.SILENT: