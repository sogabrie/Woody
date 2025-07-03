# Main Variables
NAME = libft.a
CC = gcc
RM = rm -f
MAKE = make
CFLAGS = -Wall -Wextra -Werror
LIBS = -I ../includes/
SRCS = ${wildcard *.c}
OBJS = ${SRCS:.c=.o}

# Compile Part
all : $(NAME)
	@echo > /dev/null

${NAME} : ${OBJS} Makefile ${SRCS}
	@${RM} ${NAME}
	@ar rcs ${NAME} ${OBJS}
	@${MAKE} LIBFT_FINISH

%.o : %.c
	@${CC} ${CFLAGS} ${LIBS} -c $< -o $@

clean :
	@${RM} ${OBJS}

fclean : clean
	@${RM} ${NAME}

re : fclean ${NAME} 

.PHONY : all clean fclean re check

# Helper messages
LIBFT_FINISH :
	@echo "${BLUE}Compiling LIBFT..${RESET}"

# ANSI Colors
RESET  = \033[0m
BLUE   = \033[34m