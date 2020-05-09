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

SRC		=	test_main.c canon.c canon1.c input_parser2.c lst_funcs.c \
			built_exit.c built_pwd.c built_env.c backspace.c left_right_key.c \
			mx_ush_loop.c mx_ush_execute.c mx_ush_launch.c input_parser3.c \
			mx_ush_split_line.c built_echo.c built_export.c built_cd.c \
			built_unset.c mx_cd_l.c mx_cd_p.c built_which.c \
			built_which1.c signals.c expansions.c otherbuilt.c otherbuilt1.c \
			strsplit.c replace_arg_with_arr.c parser_main.c additional_env.c \
			reparse_input_env.c env_main.c print_escape.c repl_var_with_value.c \
			get_var_from_str.c split_by_quotes.c count_quotes.c spawn_proc.c \
			repl_quotes_with_cmd.c get_cmd_output.c

SRCS	=	$(addprefix $(SRCD)/, $(SRC))
OBJS	=	$(addprefix $(OBJD)/, $(SRC:%.c=%.o))

all: $(LMXA) $(NAME)

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
