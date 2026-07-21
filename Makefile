NAME = ft_malcolm
BIN_NAME = mitm

SRC_DIR = src
OBJ_DIR = obj

LIBFT_PATH = libft/
LIBFT = $(addprefix $(LIBFT_PATH), libft.a)

SRC = $(shell find $(SRC_DIR) -name '*.c')
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

COMPILER = gcc
COMPILE_FLAGS = -Wall -Wextra -Werror -g

INSTALL_DIR = /home/$(USER)/.local/bin

#VALGRIND
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all
SUPP_FILE = getaddrinfo.supp
SUPPRESSION = --suppressions=$(SUPP_FILE)
VALGRIND = valgrind $(VALGRIND_FLAGS) $(SUPPRESSION)

RED    = \033[0;31m
GRN    = \033[0;32m
YEL    = \033[0;33m
BLU    = \033[0;34m
MAG    = \033[0;35m
CYN    = \033[0;36m
WHT    = \033[0;37m
CRESET = \033[0m

ASCII_ART = @printf "$(WHT)\n"; \
	printf "\t▗▄▄▄▖▗▄▄▄▖    ▗▖  ▗▖ ▗▄▖ ▗▖    ▗▄▄▖ ▗▄▖ ▗▖   ▗▖  ▗▖    \n"; \
	printf "\t▐▌     █      ▐▛▚▞▜▌▐▌ ▐▌▐▌   ▐▌   ▐▌ ▐▌▐▌   ▐▛▚▞▜▌    \n"; \
	printf "\t▐▛▀▀▘  █      ▐▌  ▐▌▐▛▀▜▌▐▌   ▐▌   ▐▌ ▐▌▐▌   ▐▌  ▐▌    \n"; \
	printf "\t▐▌     █      ▐▌  ▐▌▐▌ ▐▌▐▙▄▄▖▝▚▄▄▖▝▚▄▞▘▐▙▄▄▖▐▌  ▐▌    \n"; \
	printf "                                                       \n"; \
	printf "$(CRESET)"; \
	printf "$(CYN)"; \
	printf "  ___   _      ___   _      ___   _      ___   _      ___   _\n"; \
	printf " [(_)] |=|    [(_)] |=|    [(_)] |=|    [(_)] |=|    [(_)] |=|\n"; \
	printf "  '-\`  |_|     '-\`  |_|     '-\`  |_|     '-\`  |_|     '-\`  |_|\n"; \
	printf " /mmm/  /     /mmm/  /     /mmm/  /     /mmm/  /     /mmm/  /\n"; \
	printf "       |____________|____________|____________|____________|\n"; \
	printf "                             |            |            |\n"; \
	printf "                         ___  \\_      ___  \\_      ___  \\_\n"; \
	printf "                        [(_)] |=|    [(_)] |=|    [(_)] |=|\n"; \
	printf "                         '-\`  |_|     '-\`  |_|     '-\`  |_|\n"; \
	printf "                        /mmm/        /mmm/        /mmm/\n"; \
	printf "$(CRESET)\n"; \
	printf "\t$(WHT)[ which one will be poisoned?⚱ ]$(CRESET)\n\n"


all: $(NAME)

$(NAME): $(OBJ)
	@printf "$(YEL)Compiling$(CRESET) -> $(WHT)LIBFT$(CRESET)\n"
	@make -C $(LIBFT_PATH)
	@printf "$(GRN)Done$(CRESET)\n"
	@printf "$(YEL)Compiling$(CRESET) -> $(WHT)$(NAME)$(CRESET)\n"
	$(COMPILER) $(COMPILE_FLAGS) $(OBJ) $(LIBFT) -o $(NAME)
	@mkdir -p $(INSTALL_DIR)
	@cp $(NAME) $(INSTALL_DIR)/$(BIN_NAME)
# 	@export PATH=PATH:$(INSTALLDIR)/$(BIN_NAME)
	$(ASCII_ART)
	# @sudo chmod +x $(NAME)
	@printf "$(GRN)Installed ->$(CRESET) $(WHT)$(INSTALL_DIR)/$(BIN_NAME)$(CRESET)\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(COMPILER) $(COMPILE_FLAGS) -c $< -o $@

run: all
	./$(NAME) -v fedora aa:bb:cc:dd:ee:ff 192.168.1.60 00:0c:29:2b:b5:bf

val: all
	$(VALGRIND) ./$(NAME) -v fedora aa:bb:cc:dd:ee:ff 192.168.1.60 00:0c:29:2b:b5:bf
	
help:
	@printf "\nrules:\n"
	@printf "  $(GRN)%-8s$(CRESET) compiles the program and all the necessary libraries\n" "all"
	@printf "  $(GRN)%-8s$(CRESET) compiles and runs the program automatically (args are required)\n" "run"
	@printf "  $(GRN)%-8s$(CRESET) cleans the [$(YEL).o$(CRESET)] object files from the [$(YEL)obj/$(CRESET)] folder\n" "clean"
	@printf "  $(GRN)%-8s$(CRESET) calls [$(YEL)clean$(CRESET)] and removes the executable\n" "fclean"
	@printf "  $(GRN)%-8s$(CRESET) calls [$(YEL)fclean$(CRESET)] then [$(YEL)all$(CRESET)]\n" "re"
	@printf "\n"


clean:
	@printf "$(MAG)Cleaning$(CRESET) -> $(WHT)LIBFT$(CRESET)\n"
	@make -C $(LIBFT_PATH) clean
	@printf "$(MAG)Cleaning$(CRESET) -> $(WHT)OBJS$(CRESET)\n"
	@rm -rf $(OBJ_DIR)
	@printf "$(GRN)CLEAN COMPLETED$(CRESET)\n"

fclean: clean
	@make -C $(LIBFT_PATH) fclean
	@rm -f $(NAME)
	@rm -f $(INSTALL_DIR)/$(BIN_NAME)
	@printf "$(GRN)FCLEAN COMPLETED$(CRESET)\n"

re: fclean all

.DEFAULT:
	@printf "\nNo rule called: '$(RED)$@$(CRESET)'\n Type: \"$(GRN)make help$(CRESET)\" for more informations\n\n"

.PHONY: all run help clean fclean re

.SILENT:
