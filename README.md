# SceneGL - Sam Pearson-Smith
### C++ | SDL2 | OpenGL

## Requirements
- CMake
- OpenGL 3.3+
``` bash
    sudo apt install cmake pkg-config   # installs build tools
    sudo apt install mesa-utils libglu1-mesa-dev freeglut3-dev mesa-common-dev llvm  # installs opengl libs
    sudo apt install vulkan-tools libvulkan-dev    # install vulkan libs
    sudo apt install libglew-dev libsdl2-dev    # window/low-level access libs
    export VK_ICD_FILENAMES="" # disable vulkan cause its breaking stuff on WSL
```

## Summary
A simple OpenGL rendering engine for experimenting with graphics and game engine features.

Goals:

### Tasks

### High Level Operation
- [x] Create Module for main abstract modules to inherit
- [x] Implement each module to boot & shutdown
- [x] Implement each Interactive (game/simulation/editor) to start, update, stop
- [ ] Allow interactive to be chosen at runtime

### Engine/Editor Abstraction
- [ ] Editor should require project to open
- [x] Editor should be extracted from engine as seperate layer
- [ ] Implement operator that runs a given project (with game engine ticks and seperate gui ticks)

### Rendering
- [x] Implement OpenGL rasterization
- [x] Create graphics layer interface to abstract OpenGL implementation
- [x] Support lighting
- [x] Implement Point Lights
- [x] Implement Directional Lights
- [x] Implement Spot Lights
- [x] Support materials
- [ ] Support textures
- [x] Wireframe rendering
- [x] Impl shader loader as asset_loader
- [x] Create renderer interface
- [x] Use opengl calls directly in OpenGL rasterizer
- [x] Remove gl_interface

### Camera
- [x] Support cameras
- [x] Support camera movement
- [ ] Render all cameras in scene
- [ ] Show camera preview
- [ ] Show camera depth preview
- [ ] Show camera stencil preview

### Physics
- [ ] Implement AABB collision detection
- [ ] Implement Sphere collision detection
- [ ] Implement RigidBody
- [ ] Implement collision reponse/fixing
- [ ] Implement raycasting support
- [ ] Draw physics

### Entity/Components
- [x] Implement entities
- [x] Implement components
- [x] Implement entity-component engine
- [ ] Implement scene tree
- [x] Support empty enities
- [x] Pass as references not pointers
- [x] Support build pattern creation
- [x] Children in JSON
- [x] Ensure all components follow single responsibility
- [x] Implement using entt
- [x] Use smart pointers
- [ ] Implement 2d gizmos per component

### Asset Manager
- [x] Implement asset manager
- [x] Use smart pointers
- [x] Implement generic, centralise all assets
- [x] Store each asset type in its own array
- [ ] Modify asset manager to store generic types

### Asset Loaders
- [ ] Support project loading and saving
- [x] Support scene loading and saving
- [x] Support json files
- [ ] Support audio files
- [ ] Support image files
- [ ] Support text files
- [x] Support shader files
- [x] Support 3D model files
- [ ] Support animation files

### Editor Camera
- [x] Implement editor camera
- [ ] Inherit from general camera

### Shaders
- [ ] Live shader editing

### Scene
- [ ] Implement scene manager
- [ ] Allow multiple scene preloaded
- [ ] Support dynamic scene switching
- [x] Load/save scene to file
- [x] Handle entity management

### Serialization
- [x] Serialize entities
- [x] Serialize scene
- [ ] Serialize assets
- [ ] Serialize project settings

### GUI
- [x] Implement ImGui
- [x] Support dynamic gui loading with interfaces
- [ ] Create resource preview
- [ ] Create asset manager
- [ ] Create resource viewer(s)
- [ ] Remove Imguizmo
Note: ImGui is immediate mode and simple to use because it doesn't require tons of state systems on the backend.
This means you should handle it, not that you should avoid having state with components/sub-components of the gui.

### Scripting
- [ ] Create scripting engine
- [ ] Support start and update functions
- [ ] Create gizmo rendering
- [ ] Implement python scripting support
- [ ] Implement Lua scripting support

### OS Support
- [x] Support Linux
- [ ] Support Windows

### Experimental
- [ ] Implement raycasting/raytracing rendering
- [ ] Implement radiosity lighting


## Compilation
Run compile script:
```
./compile.sh clean
```

Or use CMake/Make directly:
```
cmake -S . -B build
cmake --build build
```

## Usage
Run executable:
```
./build/graphics
```

## License
This project is licensed under the [Apache-2.0](https://www.apache.org/licenses/LICENSE-2.0).

