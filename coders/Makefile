# ==============================================================
#							CODEXION
# ==============================================================

NAME			:= codexion

SRC_DIR			:= src
INCL_DIR		:= .
BUILD_DIR		:= .build
OBJ_DIR			:= $(BUILD_DIR)/objs
DEP_DIR			:= $(BUILD_DIR)/deps

CC				:= cc
CFLAGS			= -Wall -Werror -Wextra -pthread -I$(INCL_DIR) -g

SRCS :=			main.c \
				handle_args.c

SRCS_PATH		:= $(addprefix $(SRC_DIR)/,$(SRCS))
OBJS			:= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS_PATH))
DEPS			:= $(patsubst $(OBJ_DIR)/%.o,$(DEP_DIR)/%.d,$(OBJS))

RED				:= \033[1;31m
GREEN			:= \033[1;32m
YELLOW			:= \033[1;33m
CYAN			:= \033[1;36m
RESET			:= \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(YELLOW)LINKING...$(RESET)"
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(GREEN)DONE! $(CYAN)CODEXION$(GREEN) IS READY.$(RESET)"

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(DEP_DIR):
	mkdir -p $(DEP_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) $(DEP_DIR)
	@echo "$(YELLOW)COMPILING $(GREEN)$< $(YELLOW)⮞ $(GREEN)$@$(RESET)"
	$(CC) $(CFLAGS) -o "$@" -c "$<" -MMD -MP -MF "$(DEP_DIR)/$(@F:.o=.d)"

clean:
	@echo "$(RED)CLEANING BUILD DIRECTORIES...$(RESET)"
	$(RM) -r $(BUILD_DIR)

fclean: clean
	@echo "$(RED)CLEANING ARCHIVE FILES...$(RESET)"
	$(RM) $(NAME) $(B_NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
.DEFAULT_GOAL = all
