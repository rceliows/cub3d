# Standard
NAME			= cub3d

# Directories
LIBFT_DIR		= ./libft
LIBFT			= $(LIBFT_DIR)/libft.a
MINILIBX_DIR		= ./minilibx-linux
MINILIBX		= $(MINILIBX_DIR)/libmlx_Linux.a
INC			= inc/
SRC_DIR			= src/
OBJ_DIR			= obj/

# Compiler and Flags
CC			= cc
CFLAGS			= -Wall -Wextra -Werror -I$(INC) -I$(LIBFT_DIR)/inc -I$(MINILIBX_DIR)
RM			= rm -f
AR			= ar rcs
LIBS			= $(MINILIBX) $(LIBFT) -lX11 -lXext -lm

# Source Files
CUB3D			= $(SRC_DIR)main/main.c				\
			$(SRC_DIR)main/utils.c				\
			$(SRC_DIR)main/utils_2.c			\

INIT			= $(SRC_DIR)init/init_keys.c			\
			$(SRC_DIR)init/init_map.c			\
			$(SRC_DIR)init/init_maps_utils.c		\
			$(SRC_DIR)init/init_raycaster.c			\
			$(SRC_DIR)init/init_window.c			\
			$(SRC_DIR)init/init_sprites.c			\
			$(SRC_DIR)init/init_doors.c			\

RAYCASTER		=$(SRC_DIR)raycaster/raycaster.c		\
			$(SRC_DIR)raycaster/extras.c			\
			$(SRC_DIR)raycaster/calculations.c		\
			$(SRC_DIR)raycaster/utils.c			\

MOVEMENT		=$(SRC_DIR)movement/handle_keys.c		\
			$(SRC_DIR)movement/key_binds.c			\
			$(SRC_DIR)movement/movement.c			\

RENDERING		=$(SRC_DIR)rendering/door_rendering.c		\
			$(SRC_DIR)rendering/door_rendering_utils.c	\
			$(SRC_DIR)rendering/sprite_rendering.c		\
			$(SRC_DIR)rendering/sprite_drawing.c		\

PARSE 			=$(SRC_DIR)parse/helpers.c			\
			$(SRC_DIR)parse/map_validation.c		\
			$(SRC_DIR)parse/map_validation_utils.c	\
			$(SRC_DIR)parse/parseMap.c			\
			$(SRC_DIR)parse/apply_to_map.c		\
			$(SRC_DIR)parse/copy_map.c			\
			$(SRC_DIR)parse/check_n_set_header.c	\
			$(SRC_DIR)parse/parse_map_helpers.c	\
			$(SRC_DIR)parse/verify.c			\
			$(SRC_DIR)parse/player_pos.c		\
			$(SRC_DIR)parse/parsemap_refraction.c	\

# Concatenate all source files
SRCS 			= $(CUB3D) $(INIT) $(RAYCASTER) $(MOVEMENT) $(RENDERING) $(PARSE)

# Apply the pattern substitution to each source file in SRC and produce a corresponding list of object files in the OBJ_DIR
OBJ 			= $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))

# Build rules
all: 			$(NAME)

$(NAME): 		$(LIBFT) $(MINILIBX) $(OBJ)
			@echo "Linking $(NAME)..."
			@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBS)
			@echo "Build complete!"

# Check if libft directory exists, if not clone it
$(LIBFT_DIR):
			@echo "libft not found. Cloning from repository..."
			@git clone git@github.com:rceliows/libft.git $(LIBFT_DIR)
			@echo "libft cloned successfully!"

$(LIBFT):		$(LIBFT_DIR)
			@echo "Building libft..."
			@make -C $(LIBFT_DIR)
			@echo "libft built successfully!"

# Check if minilibx-linux directory exists, if not clone it
$(MINILIBX_DIR):
			@echo "MinilibX not found. Cloning from repository..."
			@git clone https://github.com/42paris/minilibx-linux.git $(MINILIBX_DIR)
			@echo "MinilibX cloned successfully!"

$(MINILIBX):		$(MINILIBX_DIR)
			@echo "Building MinilibX..."
			@make -C $(MINILIBX_DIR)
			@echo "MinilibX built successfully!"

# Compile object files from source files
$(OBJ_DIR)%.o:		$(SRC_DIR)%.c
			@mkdir -p $(@D)
			@$(CC) $(CFLAGS) -c $< -o $@

clean:
			@echo "Cleaning object files..."
			@$(RM) -r $(OBJ_DIR)
			@if [ -d "$(LIBFT_DIR)" ]; then make clean -C $(LIBFT_DIR); fi
			@if [ -d "$(MINILIBX_DIR)" ]; then make clean -C $(MINILIBX_DIR) 2>/dev/null || true; fi
			@echo "Clean complete!"

fclean: 		clean
			@echo "Removing executables..."
			@$(RM) $(NAME)
			@if [ -d "$(LIBFT_DIR)" ]; then $(RM) $(LIBFT); fi
			@echo "Full clean complete!"

fcleanall:		fclean
			@echo "Removing library directories..."
			@if [ -d "$(LIBFT_DIR)" ]; then rm -rf $(LIBFT_DIR); echo "Removed $(LIBFT_DIR)"; fi
			@if [ -d "$(MINILIBX_DIR)" ]; then rm -rf $(MINILIBX_DIR); echo "Removed $(MINILIBX_DIR)"; fi
			@echo "Full clean with libraries complete!"

re: 			fclean all

reall:			fcleanall all

# Phony targets represent actions not files
.PHONY: 		all clean fclean fcleanall re reall