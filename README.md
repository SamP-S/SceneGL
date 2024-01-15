# SceneGL - Sam Pearson-Smith
### C++ | SDL2 | OpenGL

## Summary
A simple OpenGL rendering engine for experimenting with graphics and game engine features.

Goals:
    ## Tasks

    ### Rendering
    - [x] Implement OpenGL rasterization
    - [x] Create graphics layer interface to abstract OpenGL implementation
    - [x] Support lighting
    - [x] Implement Point Lights
    - [x] Implement Directional Lights
    - [ ] Implement Spot Lights
    - [ ] Support materials
    - [ ] Support textures
    - [ ] Wireframe rendering

    ### Camera
    - [x] Support cameras
    - [x] Support camera movement
    - [ ] Render all cameras in scene
    - [ ] Show camera preview

    ### Physics
    - [ ] Implement AABB collision detection
    - [ ] Implement collision reponse/fixing
    - [ ] Implement raycasting support
    - [ ] Draw physics

    ### Entity/Components
    - [x] Implement entities
    - [x] Implement components
    - [x] Implement entity-component engine
    - [x] Implement scene tree
    - [x] Support empty enities
    - [ ] Pass as references not pointers
    - [ ] Support build pattern creation
    - [ ] Children in JSON

    ### Resources
    - [x] Implement general resource manager
    - [ ] Use integer id handle system
    - [ ] Force all instance creation through resource manager
    - [ ] Resource manager built into resource

    ### Assets
    - [x] Implement asset manager
    - [ ] Support audio files
    - [ ] Support image files
    - [ ] Support text files
    - [x] Support 3D model files
    - [ ] Support animation files

    ### Shaders
    - [ ] Live shader editing

    ### Scene
    - [ ] Implement scene manager
    - [ ] Load/save scene to file

    ### GUI
    - [x] Implement ImGui
    - [ ] Support dynamic gui loading with interfaces

    ### Networking
    - [ ] Implement networking socket support

    ### Scripting
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

