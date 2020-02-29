NAME	=	ush

CFLG	=	-std=c11 $(addprefix -W, all extra pedantic) -g

SRCD	=	src
INCD	=	inc
OBJD	=	obj

LMXD	=	libmx
LMXA:=	$(LMXD)/libmx.a
LMXI:=	$(LMXD)/$(INCD)

INC		=	ush.h
INCS	=	$(addprefix $(INCD)/, $(INC))

SRC		=	test_main.c canon.c input_parser.c lst_funcs.c built_exit.c \
			built_pwd.c built_env.c history_and_backspace.c left_right_key.c \
			mx_ush_loop.c mx_ush_execute.c mx_ush_launch.c mx_ush_split_line.c \
			built_echo.c built_export.c built_cd.c

SRCS	=	$(addprefix $(SRCD)/, $(SRC))
OBJS	=	$(addprefix $(OBJD)/, $(SRC:%.c=%.o))

all: install

install: $(LMXA) $(NAME)

$(NAME): $(OBJS)
	@clang $(CFLG) $(OBJS) -L$(LMXD) -lmx -o $@
	@printf "\r\33[2K$@\t   \033[32;1mcreated\033[0m\n"

$(OBJD)/%.o: $(SRCD)/%.c $(INCS)
	@clang $(CFLG) -c $< -o $@ -I$(INCD) -I$(LMXI)
	@printf "\r\33[2K$(NAME)\t   \033[33;1mcompile \033[0m$(<:$(SRCD)/%.c=%) "

$(OBJS): | $(OBJD)

$(OBJD):
	@mkdir -p $@

$(LMXA):
	@make -sC $(LMXD)

clean:
	@make -sC $(LMXD) $@
	@rm -rf $(OBJD)
	@printf "$(OBJD)\t   \033[31;1mdeleted\033[0m\n"

uninstall: clean
	@make -sC $(LMXD) $@
	@rm -rf $(NAME)
	@printf "$(NAME)\t   \033[31;1muninstalled\033[0m\n"

reinstall: uninstall install
