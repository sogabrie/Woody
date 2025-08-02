# Main Variables
NAME = woody_woodpacker
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
SUBDIRS = main parsing errors chreat

# Folder Directions
SRC_DIR = ${foreach dir, ${SUBDIRS}, ${addprefix ${SRC}/, ${dir}}}
OBJ_DIR = ${foreach dir, ${SUBDIRS}, ${addprefix ${OBJ}/, ${dir}}}

# File Directions
SRCS_C = ${foreach dir, ${SRC_DIR}, ${wildcard ${dir}/*.c}}
SRCS_S = ${foreach dir, ${SRC_DIR}, ${wildcard ${dir}/*.s}}
OBJS_C = ${subst ${SRC}, ${OBJ}, ${SRCS_C:.c=.o}}
OBJS_S = ${subst ${SRC}, ${OBJ}, ${SRCS_S:.s=.o}}
OBJS = ${OBJS_C} ${OBJS_S}
LIB_DIR = ${foreach dir, ${LIBS}, ${wildcard ${dir}/*.h}}

ifeq ($(ARCH_MODE), 32)
    ARCH_FLAGS = -m32
    NASM_FORMAT = elf
    TARGET_SUFFIX = _x32
else
    ARCH_FLAGS =
    NASM_FORMAT = elf64
    TARGET_SUFFIX = _x64
endif

# Main Part
all : ${NAME}${TARGET_SUFFIX}

${NAME}${TARGET_SUFFIX} : ${LIB_DIR} Makefile ${OBJS} ${SRCS} ${LIBFT_ALL}
	${MAKE} -C ${LIBFT_DIR} all CFLAGS="${ARCH_FLAGS}"
	cc -g ${FLAGS} ${ARCH_FLAGS} ${LIBS_INCLUDE} ${OBJS} ${LIBS_PATH} -o ${NAME}${TARGET_SUFFIX}

${OBJ}/%.o : ${SRC}/%.c ${LIB_DIR}
	mkdir -p ${OBJ} ${OBJ_DIR}
	cc ${FLAGS} ${ARCH_FLAGS} ${LIBS_INCLUDE} -c $< -o $@

${OBJ}/%.o : ${SRC}/%.s
	mkdir -p ${OBJ} ${OBJ_DIR}
	nasm -f ${NASM_FORMAT} -o $@ $<

clean :
	${MAKE} -C ${LIBFT_DIR} clean
	${RM} ${OBJ}

fclean : clean
	${MAKE} -C ${LIBFT_DIR} fclean
	${RM} ${NAME}_x64 ${NAME}_x32 # Удаляем обе версии исполняемого файла

re : fclean 
	${MAKE} all

.PHONY : all clean fclean re
# make ARCH_MODE=32