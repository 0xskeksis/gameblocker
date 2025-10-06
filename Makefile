NAME	?= gameblocker

CC	:=	clang

CFLAGS	:= -g

SRCS	?=  src/*.c

BUILD	:= .objs

OBJS	:= $(addprefix $(BUILD)/, $(SRCS:%.c=%.o))

MAKEFLAGS	+= --no-print-directory

RM			:= rm -rf

DIR_UP		= mkdir -p $(@D)

################################################################################

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@
	@printf " $(GREEN)$(BOLD)$(ITALIC)■$(RESET)  building	$(GREEN)$(BOLD)$(ITALIC)$(NAME)$(RESET)\n"

$(BUILD)/%.o: %.c
	@$(DIR_UP)
	@printf " $(CYAN)$(BOLD)$(ITALIC)■$(RESET)  compiling	$(GRAY)$(BOLD)$(ITALIC)$^$(RESET)\n"
	@$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@printf " $(RED)$(BOLD)$(ITALIC)■$(RESET)  deleting	$(GRAY)$(BOLD)$(ITALIC)$(BUILD)$(RESET)\n"
	@$(RM) $(BUILD)

fclean: clean
	@printf " $(RED)$(BOLD)$(ITALIC)■$(RESET)  deleting	$(GRAY)$(BOLD)$(ITALIC)$(NAME)$(RESET)\n"
	@$(RM) $(NAME)

re: fclean all

################################################################################


BLACK		=	\033[30m
RED			=	\033[31m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
MAGENTA		=	\033[35m
CYAN		=	\033[36m
WHITE		=	\033[37m
GRAY		=	\033[90m

BOLD		=	\033[1m
ITALIC		=	\033[3m

RESET		=	\033[0m
LINE_CLR	=	\33[2K\r
