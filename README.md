# cub3D

A 3D raycasting game engine inspired by Wolfenstein 3D, built with MinilibX and written in C.

![cub3D](https://img.shields.io/badge/Language-C-blue)
![42 Project](https://img.shields.io/badge/42-Project-black)

## Table of Contents

- [Description](#description)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Map Format](#map-format)
- [Controls](#controls)
- [Bonus Features](#bonus-features)
- [Error Handling](#error-handling)
- [Known Issues](#known-issues)
- [Project Structure](#project-structure)

## Description

cub3D is a raycasting engine that renders a 3D maze from a first-person perspective. Using the raycasting technique made famous by games like Wolfenstein 3D, this project creates a pseudo-3D environment where players can navigate through corridors rendered with textured walls.

## Features

### Core Features
- **Raycasting engine** - Real-time 3D rendering using 2D map data
- **Textured walls** - Different textures for each cardinal direction (North, South, East, West)
- **Customizable colors** - Floor and ceiling colors defined in RGB
- **Smooth movement** - Frame-rate independent movement and rotation
- **Map validation** - Comprehensive parsing and validation of map files

### Bonus Features
- **Doors** - Interactive doors that open with the 'E' key and auto-close after 3 seconds
- **Sprites** - Static and animated sprites in the game world
- **Minimap** - Toggle-able overhead view of the current level
- **Mouse controls** - Look around with mouse movement
- **FPS counter** - Display current frame rate
- **Vertical looking** - Pitch control for looking up and down

## Requirements

- Linux operating system
- gcc compiler
- make
- X11 development libraries
- XExt development libraries

### Installing Dependencies (Debian/Ubuntu)

```bash
sudo apt-get update
sudo apt-get install build-essential libx11-dev libxext-dev
```

## Installation

1. **Clone the repository**
   ```bash
   git clone <repository-url> cub3d
   cd cub3d
   ```

2. **Build the project**
   ```bash
   make
   ```

   This will automatically:
   - Clone and build the libft library
   - Clone and build MinilibX
   - Compile all source files
   - Link everything into the `cub3d` executable

3. **Clean build files** (optional)
   ```bash
   make clean      # Remove object files
   make fclean     # Remove object files and executable
   make fcleanall  # Remove everything including libraries
   make re         # Rebuild everything
   ```

## Usage

Run the game with a map file:

```bash
./cub3d maps/good/subject_map.cub
```

### Exit Codes

The program returns different exit codes based on the type of error:
- **0**: Success (normal execution)
- **1**: Configuration errors (invalid map format, missing textures, validation failures)
- **2**: File access errors (map file not found, texture files not readable, permission denied)

### Available Maps

**Good Maps** (Valid configurations):
- `maps/good/subject_map.cub` - The example map from the project subject
- `maps/good/test_map.cub` - Simple test map with spaces
- `maps/good/cheese_maze.cub` - Complex maze layout
- `maps/good/dungeon.cub` - Dungeon-themed map
- `maps/good/library.cub` - Library-themed environment
- `maps/good/creepy.cub` - Creepy atmosphere with sprites

**Bad Maps** (For testing error handling):
- Various maps in `maps/bad/` testing different validation cases

## Map Format

Maps are defined in `.cub` files with the following format:

### Texture Definitions
```
NO ./textures/north_texture.xpm
SO ./textures/south_texture.xpm
WE ./textures/west_texture.xpm
EA ./textures/east_texture.xpm
```

### Color Definitions
```
F 220,100,0    # Floor color (RGB)
C 225,30,0     # Ceiling color (RGB)
```

### Map Layout
```
111111111111
100000000001
100N00000001
100000000001
111111111111
```

#### Map Characters
- `0` - Empty space (walkable)
- `1` - Wall
- `N/S/E/W` - Player start position and orientation
- `D` - Door (bonus)
- `P` - Static sprite (bonus)
- `A` - Animated sprite (bonus)
- ` ` (space) - Void (must be surrounded by walls)

### Validation Rules
1. Map must be surrounded by walls
2. Exactly one player start position required
3. All textures must be valid .xpm files
4. RGB values must be between 0-255
5. No gaps in the map boundaries
6. No duplicate texture or color definitions

## Controls

### Movement
- `W` - Move forward
- `S` - Move backward
- `A` - Strafe left
- `D` - Strafe right

### Camera
- `←` - Rotate camera left
- `→` - Rotate camera right
- **Mouse movement** - Look around (horizontal and vertical)

### Actions
- `E` - Interact with doors (open/close)

### Display Options
- `M` - Toggle minimap
- `F` - Toggle FPS counter
- `Q` - Toggle mouse lock

### Exit
- `ESC` - Exit game
- Click **X** on window - Exit game

## Bonus Features

### Interactive Doors
- Marked with `D` in the map
- Open with the `E` key when nearby
- Automatically close after 3 seconds
- Stay open if player is standing in doorway

### Sprites
- **Static sprites** (`P`) - Decorative objects with billboard rendering
- **Animated sprites** (`A`) - Objects with frame-based animation
- Proper depth sorting for correct rendering
- Transparent pixel support

### Minimap
- Toggle with `M` key
- Shows overhead view of the level
- Player position marked in red
- Walls shown in white
- Doors shown in brown
- Sprites shown in green/yellow

### Mouse Look
- Full mouse control for camera rotation
- Vertical pitch control (look up/down)
- Toggle mouse lock with `Q` key
- Centered cursor for smooth looking

## Error Handling

The program provides detailed error messages for various validation failures:

### Map File Errors
- `Error: file must have .cub extension` - Map file doesn't end with .cub
- `Error: No such file or directory` - Map file not found
- `Error: Permission denied` - Cannot read map file

### Header Errors
- `Error: missing NO/SO/WE/EA texture` - Texture definition not found
- `Error: duplicate NO/SO/WE/EA texture` - Texture defined multiple times
- `Error: NO/SO/WE/EA texture must be .xpm file` - Wrong file extension
- `Error: NO/SO/WE/EA texture: No such file or directory` - Texture file not found
- `Error: missing floor/ceiling color` - Color definition not found
- `Error: duplicate floor/ceiling color` - Color defined multiple times
- `Error: floor/ceiling color has no value` - Empty color definition
- `Error: invalid floor/ceiling color format` - Wrong RGB format or values out of range (0-255)

### Map Errors
- `Error: invalid character in map` - Map contains invalid characters
- `Error: empty line in middle of map` - Blank line found within map boundaries
- `Error: no player found` - No starting position (N/S/E/W) in map
- `Error: multiple player found` - More than one starting position
- `Error: fails map check` - Map not properly enclosed by walls

## Known Issues

### Memory Leaks from MinilibX

Due to the use of `mlx_mouse_hide()` function from MinilibX, there are unavoidable memory leaks reported by Valgrind. These leaks originate from X11/Xcursor libraries used internally by MinilibX and are beyond the scope of this project to fix.

**Example Valgrind output:**
```
==44079== HEAP SUMMARY:
==44079==     in use at exit: 31,567 bytes in 400 blocks
==44079==   total heap usage: 3,062 allocs, 2,662 frees, 1,137,732 bytes allocated
```

These leaks are:
- **Not caused by the cub3D codebase** - They originate from MinilibX's interaction with X11
- **System library related** - Specifically from `libX11.so` and `libXcursor.so`
- **Unavoidable when using mouse functions** - There is no MinilibX function to properly clean up these allocations

**Note:** All memory allocated by the cub3D program itself is properly freed. The remaining "still reachable" blocks are internal to the graphics libraries and are automatically cleaned up by the operating system when the program exits.

## Project Structure

```
cub3d/
├── inc/
│   └── cub3d.h              # Main header file
├── src/
│   ├── init/                # Initialization functions
│   ├── main/                # Main loop and utilities
│   ├── movement/            # Player movement and controls
│   ├── parse/               # Map parsing and validation
│   ├── raycaster/           # Raycasting engine
│   └── rendering/           # Sprite and door rendering
├── maps/
│   ├── good/                # Valid test maps
│   └── bad/                 # Invalid maps for testing
├── textures/                # XPM texture files
├── Makefile                 # Build configuration
└── README.md               # This file
```

### Key Components

- **Raycasting Engine** - Core 3D rendering using DDA algorithm
- **Map Parser** - Validates and loads .cub files with comprehensive error checking
- **Sprite System** - Billboard-based sprite rendering with sorting
- **Door Manager** - State management for interactive doors
- **Input Handler** - Keyboard and mouse event processing

## Technical Details

### Raycasting
The engine uses the Digital Differential Analyzer (DDA) algorithm to cast rays for each vertical screen column, determining wall distances and heights for perspective rendering.

### Frame Rate Independence
Movement and rotation speeds are multiplied by frame time to ensure consistent behavior regardless of FPS.

### Texture Mapping
Walls use proper texture coordinate calculation to avoid distortion and support different texture sizes.

### Error Recovery
The parser implements early-exit validation, stopping at the first error encountered and providing specific error messages to aid debugging.

## Credits

Project developed as part of the 42 school curriculum.

- **Graphics Library**: MinilibX
- **Utility Library**: libft (custom C library)

## License

This project is part of the 42 school curriculum and follows their academic policies.
