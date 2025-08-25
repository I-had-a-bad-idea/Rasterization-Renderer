# Rasterization-Renderer

A simple rasterization-based 3D renderer built with C++

## Features

- **Rasterization Rendering:** Basic 3D rendering using rasterization.
- **Scene Management:** Support for loading 3D objects (in ".obj" format) and setting up a scene with multiple objects.
- **Camera Movement:** Simple first-person camera with mouse and keyboard control.
- **Basic Shading:** Supports both texture-based and lit shading for objects.

## Requirements

- C++17 or later
- SDL2 or later (for window handling)
- A C++ compiler (e.g., GCC, Clang, or MSVC)

## Installation

1. Clone this repository:
    ```bash
    git clone https://github.com/I-had-a-bad-idea/Rasterization-Renderer.git
    cd rasterization-renderer
    ```

#TODO find out how this should be done
2. Compile the project using your preferred C++ compiler. If you're using `g++`:
    ```bash
    
    ```

## Usage

### Running the Renderer

To run the renderer, simply execute the compiled executable:

#TODO again find the right command
```bash

```

This will open a window to which the scene is rendered. The basic controls are:

- **W, A, S, D:** Move the camera forward, left, backward, and right.
- **Middle Mouse Buttn + Mouse movement:** Look around (drag to rotate the camera).


The renderer will render a scene that includes a floor, a monkey model, and a few cubes. You can add more objects or modify the existing ones by editing the TestScene class.


## Project Structure
#TODO add the project structure

### Main Classes and Functions:

- **Rasterizer**: Handles the actual rendering process, including clearing the screen, drawing triangles, depth testing, and shading.
- **Math**: A class dealing with most of the math
- **Object**: Represents a 3D object, containing its mesh, transformations, and shaders.
- **ObjectMesh**: Contains vertices, normals, and texture coordinates for a 3D model.
- **Shaders**: Implements the shaders used for rendering, including texture sampling and lighting calculations.
- **TestScene**: Defines the scene setup, including objects and camera configurations.
- **ObjectTransform**: Handles the transformation of objects between world and local space.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
