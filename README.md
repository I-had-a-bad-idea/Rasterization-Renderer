# Rasterization-Renderer

A simple software rasterization-based 3D renderer built with C++ and SDL2.

## Features

- **Rasterization Rendering:** Fast CPU-based 3D rendering using rasterization
- **Multi-threaded Rendering:** Optimized performance using thread pools
- **Scene Management:** Support for loading 3D objects (in .obj format)
- **Camera Controls:** First-person camera with mouse and keyboard input
- **Texture Support:** Both basic and lit texture shading
- **Performance:** Real-time rendering with FPS counter

## Requirements

- C++17 or later
- SDL2 and SDL2_image
- OpenGL (for display only)
- CMake 3.10 or later (for building)
- A C++ compiler (GCC, Clang, or MSVC)

## Installation

### Windows (MSYS2)

1. Install MSYS2 from https://www.msys2.org/

2. Open MSYS2 UCRT64 terminal and install dependencies:
```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-SDL2
pacman -S mingw-w64-ucrt-x86_64-SDL2_image
```

3. Clone and build the project:
```bash
git clone https://github.com/I-had-a-bad-idea/Rasterization-Renderer.git
cd Rasterization-Renderer
g++ -O3 -I. -I./Helper -I./Math -I./Object -I./Rendering -I./Scenes ./*.cpp ./Helper/*.cpp ./Math/*.cpp ./Object/*.cpp ./Rendering/*.cpp ./Scenes/*.cpp -lSDL2 -lSDL2_image -lopengl32 -o renderer.exe
```

## Usage

Run the compiled executable:
```bash
./renderer.exe
```

### Controls
- **W, A, S, D:** Move the camera forward, left, backward, and right
- **Middle Mouse Button + Mouse:** Look around (drag to rotate camera)
- **ESC:** Exit the application

## Project Structure

```
Rasterization-Renderer/
├── Helper/           # Utility functions and helpers
├── Math/            # Math operations and vector types
├── Object/          # 3D object and mesh handling
├── Rendering/       # Core rendering functionality
├── Scenes/          # Scene management and test scenes
└── main.cpp         # Application entry point
```

### Core Components

- **Rasterizer:** Main rendering pipeline implementation
- **RenderTarget:** Frame buffer and depth buffer management
- **Object:** 3D model representation with transformation
- **Scene:** Scene graph and object management
- **Camera:** View frustum and camera movement
- **Shader:** Basic and lit texture shading implementations

## Performance

The renderer uses multi-threading to improve performance:
- Thread pool for triangle rasterization
- Optimized vertex transformation
- Efficient back-face culling
- Fast depth testing

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
