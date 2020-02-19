NAME = ush

INC = ush.h
OBJH = obj/uls.h

SRC = *.c

OBJ = *.o

CFLAG = -std=c11 -Wall -Wextra -Wpedantic -Werror

all: install clean

install:
	@cp $(addprefix src/, $(SRC)) .
	@cp $(addprefix inc/, $(INC)) .
	@clang $(CFLAGS) -c $(SRC) -I $(INC)
	@clang $(CFLAGS) $(OBJ) -o $(NAME)
	@mkdir -p obj
	@mv $(OBJ) ./obj
uninstall: clean
	@rm -rf $(NAME)
clean:
	@rm -rf $(INC) $(SRC) ./obj
reinstall: uninstall install
