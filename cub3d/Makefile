# Standard
NAME			= cub3d

# Directories
MINILIBX		= ./minilibx-linux/libmlx_Linux.a
INC			= inc/
SRC_DIR			= src/
OBJ_DIR			= obj/

# Compiler and Flags
CC			= cc
# CFLAGS			= -Wall -Wextra -Werror -I$(INC) -I./minilibx-linux
CFLAGS			= -Wall -Wextra -Werror -g -I$(INC) -I./minilibx-linux
RM			= rm -f
AR			= ar rcs
LIBS			= $(MINILIBX) -lX11 -lXext -lm

# Source Files
CUB3D		=	$(SRC_DIR)main.c		\
			$(SRC_DIR)raycaster.c		\
			$(SRC_DIR)key_binds.c		\
			$(SRC_DIR)utils.c		\

# Concatenate all source files
SRCS 			= $(CUB3D)

# Apply the pattern substitution to each source file in SRC and produce a corresponding list of object files in the OBJ_DIR
OBJ 			= $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))

# Build rules
start:				
			@make all

# Check if minilibx-linux directory exists, if not clone it
./minilibx-linux:
			@echo "MinilibX not found. Cloning from repository..."
			@git clone https://github.com/42paris/minilibx-linux.git
			@echo "MinilibX cloned successfully!"

$(MINILIBX):		./minilibx-linux
			@echo "Building MinilibX..."
			@make -C ./minilibx-linux
			@echo "MinilibX built successfully!"

all: 			$(NAME)

$(NAME): 		$(OBJ) $(MINILIBX)
			@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBS)

# Compile object files from source files
$(OBJ_DIR)%.o:		$(SRC_DIR)%.c ./minilibx-linux
			@mkdir -p $(@D)
			@$(CC) $(CFLAGS) -c $< -o $@

clean:
			@$(RM) -r $(OBJ_DIR)

fclean: 		clean
			@$(RM) $(NAME)

re: 			fclean all

# Phony targets represent actions not files
.PHONY: 		start all clean fclean re
