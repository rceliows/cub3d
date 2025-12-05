# Standard
NAME			= cub3d

# Directories
LIBFT			= ./libft/libft.a
MINILIBX		= ./minilibx-linux/libmlx_Linux.a
INC			= inc/
SRC_DIR			= src/
OBJ_DIR			= obj/

# Compiler and Flags
CC			= cc
CFLAGS			= -Wall -Wextra -Werror -I$(INC) -I./minilibx-linux
RM			= rm -f
RM_LIBX			= rm -r
AR			= ar rcs
LIBS			= $(MINILIBX) -lX11 -lXext -lm

# Source Files
CUB3D			= $(SRC_DIR)main/main.c			\
			$(SRC_DIR)main/utils.c			\
			$(SRC_DIR)main/cleanup_cub3d.c		\

INIT			= $(SRC_DIR)init/init_keys.c		\
			$(SRC_DIR)init/init_map.c		\
			$(SRC_DIR)init/init_raycaster.c		\
			$(SRC_DIR)init/init_window.c		\

RAYCASTER		=$(SRC_DIR)raycaster/raycaster.c	\
			$(SRC_DIR)raycaster/calculations.c	\
			$(SRC_DIR)raycaster/extras.c		\
			$(SRC_DIR)raycaster/utils.c		\

MOVEMENT		=$(SRC_DIR)movement/handle_keys.c		\
			$(SRC_DIR)movement/key_binds.c		\
			$(SRC_DIR)movement/movement.c		\

PARSE 			=$(SRC_DIR)parse/srcs/helpers.c \
			$(SRC_DIR)parse/srcs/map_validation.c \
			$(SRC_DIR)parse/srcs/parseMap.c \
			$(SRC_DIR)parse/srcs/apply_to_map.c \
			$(SRC_DIR)parse/srcs/copy_map.c		\
			$(SRC_DIR)parse/srcs/check_n_set_header.c		\
			$(SRC_DIR)parse/srcs/parse_map_helpers.c		\
			$(SRC_DIR)parse/srcs/verify.c		\
			$(SRC_DIR)parse/srcs/player_pos.c		\
			$(SRC_DIR)parse/srcs/parsemap_refraction.c		\
# Concatenate all source files
SRCS 			= $(CUB3D) $(INIT) $(RAYCASTER) $(MOVEMENT) $(PARSE)

# Apply the pattern substitution to each source file in SRC and produce a corresponding list of object files in the OBJ_DIR
OBJ 			= $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))

# Build rules
start:				
			@make all

$(LIBFT):
			@make -C ./libft

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

$(NAME): 		$(OBJ) $(LIBFT) $(MINILIBX)
			@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBFT) $(LIBS)

# Compile object files from source files
$(OBJ_DIR)%.o:		$(SRC_DIR)%.c ./minilibx-linux
			@mkdir -p $(@D)
			@$(CC) $(CFLAGS) -c $< -o $@

clean:
			@$(RM) -r $(OBJ_DIR)
			@make clean -C ./libft

fclean: 		clean
			@$(RM) $(NAME)
			@$(RM) $(LIBFT)

re: 			fclean all

# Phony targets represent actions not files
.PHONY: 		start all clean fclean re
