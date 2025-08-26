![MIT License](https://img.shields.io/badge/license-MIT-green)
![C++](https://img.shields.io/badge/c++-17+-blue)
![SDL2](https://img.shields.io/badge/SDL2-red)
![Commit Activity](https://img.shields.io/github/commit-activity/m/I-had-a-bad-idea/Rasterization-Renderer)
![Last Commit](https://img.shields.io/github/last-commit/I-had-a-bad-idea/Rasterization-Renderer)
![Open Issues](https://img.shields.io/github/issues/I-had-a-bad-idea/Rasterization-Renderer)
![Closed Issues](https://img.shields.io/github/issues-closed/I-had-a-bad-idea/Rasterization-Renderer)
![Repo Size](https://img.shields.io/github/repo-size/I-had-a-bad-idea/Rasterization-Renderer)
![Contributors](https://img.shields.io/github/contributors/I-had-a-bad-idea/Rasterization-Renderer)


# Rasterization-Renderer

A basic software rasterization-based 3D renderer built with C++ and SDL.

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
main_release.exe
```

### Controls
- **W, A, S, D:** Move the camera forward, left, backward, and right
- **Mouse:** Look around 
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

- [**Rasterizer:**](Rendering/Rasterizer.cpp) Main rendering pipeline implementation
- [**Math:**](Math/Maths.cpp) Implementation of all the math 
- [**Threadpool:**](Rendering/Threadpool.cpp) Threadpool for multithreading
- [**RenderTarget:**](Rendering/RenderTarget.h) Frame buffer and depth buffer management
- [**Object:**](Object/Object.h) 3D object with model, shader and [transform](Object/ObjectTransform.h)
- [**Mesh:**](Object/Object_mesh.cpp) Representation of a 3D model with texture
- [**Obj_Loader:**](Helper/Obj_loader.cpp) Loads an .obj file and creates a mesh
- [**Scene:**](Scenes/Scene.h) Camera and object management ([Example Scene](Scenes/Test_scene.cpp))

## Performance
-
- Thread pool for triangle rasterization
- Optimized vertex transformation
- Efficient back-face culling
- Fast depth testing

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


Created using <Metal 048 A>, <Gravel 041> <Grass 005> from ambientCG.com,
licensed under the Creative Commons CC0 1.0 Universal License.