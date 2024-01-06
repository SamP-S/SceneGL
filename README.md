# SceneGL - Sam Pearson-Smith
### C++ | SDL2 | OpenGL

## Summary
A simple OpenGL rendering engine for experimenting with graphics and game engine features.

Goals:
    ## Tasks

    ### Graphics
    - [x] Implement OpenGL rasterization
    - [x] Create graphics layer interface to abstract OpenGL implementation
    - [ ] Implement raycasting/raytracing rendering
    - [ ] Implement radiosity lighting
    - [ ] Support camera movement
    - [ ] Wireframe rendering
    - [ ] Draw physics

    ### Physics
    - [ ] Implement AABB collision detection
    - [ ] Implement collision reponse/fixing
    - [ ] Implement raycasting support

    ### Entity/Components
    - [x] Implement entities
    - [ ] Implement components
    - [ ] Implement entity-component engine
    - [x] Implement scene tree
    - [ ] Support empty enities

    ### Resources
    - [x] Implement general resource manager
    - [ ] Support audio files
    - [ ] Support image files
    - [ ] Support text files
    - [ ] Support 3D model files
    - [ ] Support animation files
    - [ ] 

    ### Networking
    - [ ] Implement networking socket support

    ### Scripting
    - [ ] Implement Lua scripting support

    ### OS Support
    - [x] Support Linux
    - [ ] Support Windows


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

