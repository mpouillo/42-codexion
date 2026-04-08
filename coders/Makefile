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

ARGS ?= 10 420 100 100 100 10 20 edf

SRCS :=			coder_actions.c		\
				coders.c 			\
				dongle_actions.c	\
				dongles.c			\
				main.c				\
				monitor.c			\
				parsing.c			\
				print.c				\
				queue.c				\
				sim.c				\
				thread.c			\
				timing.c

SRCS_PATH		:= $(addprefix $(SRC_DIR)/,$(SRCS))
OBJS			:= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS_PATH))
DEPS			:= $(patsubst $(OBJ_DIR)/%.o,$(DEP_DIR)/%.d,$(OBJS))

RED				:= \033[1;31m
GREEN			:= \033[1;32m
YELLOW			:= \033[1;33m
CYAN			:= \033[1;36m
RESET			:= \033[0m

all: $(NAME)

run: all
	./codexion $(ARGS)

test: all
	for i in {1...20}; do ./codexion $(ARGS); done

valgrind: all
	valgrind ./codexion $(ARGS)

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
