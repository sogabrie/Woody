# Main Variables
NAME = Woody
FLAGS = -Wall -Wextra -Werror
MAKE = make
ECHO = echo
RM = rm -rf
LIBS_INCLUDE = -I./includes/
LIBS = includes
LIBS_PATH = -L${LIBFT_DIR} -l${LIBFT}

# Libft Variables
LIBFT = ft
LIBFT_DIR = ./lib${LIBFT}
LIBFT_SRC := ${shell find ${LIBFT_DIR} -name '*.c' -type f}
LIBFT_MAKE = ${LIBFT_DIR}/Makefile
LIBFT_ALL = ${LIBFT_DIR} ${LIBFT_MAKE} ${LIBFT_SRC}

# Compiled Directories
SRC = src
OBJ = obj
SUBDIRS = main

# Folder Directions
SRC_DIR = ${foreach dir, ${SUBDIRS}, ${addprefix ${SRC}/, ${dir}}}
OBJ_DIR = ${foreach dir, ${SUBDIRS}, ${addprefix ${OBJ}/, ${dir}}}

# File Directions
SRCS = ${foreach dir, ${SRC_DIR}, ${wildcard ${dir}/*.c}}
OBJS = ${subst ${SRC}, ${OBJ}, ${SRCS:.c=.o}}
LIB_DIR = ${foreach dir, ${LIBS}, ${wildcard ${dir}/*.h}}

# Main Part
all : ${NAME}

${NAME} : ${LIB_DIR} Makefile ${OBJS} ${SRCS} ${LIBFT_ALL}
	@${MAKE} -C ${LIBFT_DIR} all
	@cc ${FLAGS} ${LIBS_INCLUDE} ${LIBS_PATH} ${OBJS} -o ${NAME}

${OBJ}/%.o : ${SRC}/%.c ${LIB_DIR}
	@mkdir -p ${OBJ} ${OBJ_DIR}
	@cc ${CFLAGS} ${LIBS_INCLUDE} -c $< -o $@

clean :
	@${MAKE} -C ${LIBFT_DIR} clean
	@${RM} ${OBJ}

fclean : clean
	@${MAKE} -C ${LIBFT_DIR} fclean
	@${RM} ${NAME}

re : fclean 
	@${MAKE} all

.PHONY : all clean fclean re